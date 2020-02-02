/**************************************************************************

   Copyright © 2020 Sergey Poluyan <svpoluyan@gmail.com>

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

**************************************************************************/
#ifndef MVEQF_H
#define MVEQF_H

#include "kquantile.h"
#include "mvff.h"

#include "data_io.h"
template<class InputIt, class T>
void parallel_pdff(InputIt first,
                   InputIt last,
                   std::vector<std::vector<T>> &pdf,
                   const T lb,
                   const T ub,
                   std::function<T(const std::vector<T> &)> f)
{
    const auto size = last - first;
    const auto nthreads = std::thread::hardware_concurrency();
    const auto size_per_thread = size / nthreads;

    std::vector<std::future<void>> futures;
    for(unsigned int i = 0; i < nthreads - 1; i++)
    {
        futures.emplace_back(std::async([start = first + i * size_per_thread, size_per_thread, &pdf, lb, ub, f]()
        {
            T es = ub - lb;
            for(auto it = start; it != start + size_per_thread; ++it)
            {
                size_t i = std::distance(pdf.begin(), it);
                T a = lb + i*es/(pdf.size() - 1);
                for(size_t j = 0; j != it->size(); j++)
                {
                    std::vector<T> temp = {a, lb + j*es/(pdf.size() - 1)};
                    pdf[i][j] = f(temp);
                }
            }
        }));
    }
    futures.emplace_back(
        std::async([start = first + (nthreads - 1) * size_per_thread, last, &pdf, lb, ub, f]()
    {
        T es = ub - lb;
        for(auto it = start; it != last; ++it)
        {
            size_t i = std::distance(pdf.begin(), it);
            T a = lb + i*es/(pdf.size() - 1);
            for(size_t j = 0; j != it->size(); j++)
            {
                std::vector<T> temp = {a, lb + j*es/(pdf.size() - 1)};
                pdf[i][j] = f(temp);
            }
        }

    }));

    for(auto &&future : futures)
    {
        if(future.valid())
        {
            future.get();
        }
        else
        {
            throw std::runtime_error("Something going wrong.");
        }
    }
}

template <typename T>
void print_pdfff(std::string fname, std::function<T(const std::vector<T> &)> f, const T lb, const T ub, const size_t n)
{
    std::vector<std::vector<T> > pdf(n, std::vector<T>(n));
    //parallel_step(pdf.begin(), pdf.end(), pdf, lb, ub, f);
    parallel_pdff(pdf.begin(), pdf.end(), pdf, lb, ub, f);
    data_io::write_default2d(fname, pdf, 5);
}

namespace mveqf
{

template <typename T, typename U>
class MVEQF
{
protected:
    size_t kernel_type;
    U lambda;
    std::shared_ptr<trie_based::Trie<trie_based::NodeCount<T>,T>> sample;
    std::shared_ptr<ImplicitTrieKQuantile<T, U>> qf;
public:
    MVEQF(): kernel_type(0), lambda(1.0) {}
    void set_kernel_type(size_t kt)
    {
        kernel_type = kt;
    }
    void set_sample_and_bounds(const std::vector<std::vector<U>> &init_points,
                               const std::vector<U> &lb,
                               const std::vector<U> &ub,
                               const std::vector<size_t> &number = std::vector<size_t>(),
                               const size_t N = 100,
                               const U threshold = 1e-8,
                               const size_t multi = 100000,
                               const U lambda = 1.0)
    {
        const size_t dimension = lb.size();

        auto init_sample_points = std::make_shared<std::vector<std::vector<U>>>(init_points.size(), std::vector<U>(dimension));
        for(auto it = init_sample_points->begin(); it != init_sample_points->end(); ++it)
        {
            size_t i = std::distance(init_sample_points->begin(), it);
            for(size_t j = 0; j != dimension; j++)
            {
                (*it)[j] = init_points[i][j];
            }
        }

        mveqf::kde::KDE<U> init_pdf;
        init_pdf.set_dimension(dimension);
        init_pdf.set_kernel_type(kernel_type);
        if(number.empty())
        {
            init_pdf.set_sample_shared(init_sample_points);
        }
        else
        {
            auto count_number = std::make_shared<std::vector<size_t>>(number);
            init_pdf.set_sample_shared(init_sample_points, count_number);
        }

        std::vector<size_t> gridn(lb.size(), N);
        qf = std::make_shared<ImplicitTrieKQuantile<T, U>>(lb, ub, gridn, kernel_type);

        const auto grid = qf->get_grid();
//        std::cout << grid.front().size() << std::endl;
//        std::cin.get();
//        for(const auto & i : grid)
//        {
//            for(const auto & j : i)
//            {
//                std::cout << j << '\t';
//            }
//            std::cout << std::endl;
//        }
        const auto dx = qf->get_dx();

        // finding start dot over created grid
        std::vector<std::vector<T>> points;
//        for(const auto &k : init_points)
//        {
//            std::cout << std::fixed << k.front() << '\t' << k.back() << std::endl;
//        }
        for(const auto &k : init_points)
        {
            std::vector<int> startdot(dimension);
            for(size_t i = 0; i != startdot.size(); i++)
            {
                auto it = std::lower_bound(grid[i].begin(), grid[i].end(), k[i]);

                if(it == grid[i].end())
                    startdot[i] = grid[i].size() - 2;
                else if(it == grid[i].begin())
                    startdot[i] = 0;
                else
                    startdot[i] = std::distance(grid[i].begin(), it) - 1;
            }
            points.push_back(startdot);
        }
//        for(const auto &k : points)
//        {
//            std::cout << std::fixed << k.front() << '\t' << k.back() << std::endl;
//        }
//        for(const auto &k : number)
//        {
//            std::cout << std::fixed << k << std::endl;
//        }
        std::set<std::vector<T>> visited;

        std::function<U(const std::vector<U> &)> pdffunc = std::bind(&mveqf::kde::KDE<U>::pdf, std::ref(init_pdf), std::placeholders::_1, lambda);

        ///

        //print_pdfff<U>("maps/kpdf2d.dat", pdffunc, lb.front(), ub.front(), 100);
        //std::cin.get();


        ///

        sample = std::make_shared<mveqf::trie_based::Trie<mveqf::trie_based::NodeCount<T>,T>>();
        sample->set_dimension(dimension);

//
        size_t counter = 0, fe_count = 0;
//        std::cout << "FloodFill" << std::endl;
        mveqf::mvff::FloodFill_MultipleGrids_VonNeumann_trie<U>(grid, points, sample, dx, counter, fe_count, pdffunc, threshold, multi);
//        std::cout << "FloodFill" << std::endl;

        qf->set_sample_shared(sample);
    }
    void transform(const std::vector<U>& in01, std::vector<U>& out) const
    {
        qf->transform(in01, out, lambda);
    }
    std::vector<U> transform(const std::vector<U>& in01) const
    {
        return qf->transform(in01, lambda);
    }
};

}

#endif
