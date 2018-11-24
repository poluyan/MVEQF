/**************************************************************************

   Copyright © 2018 Sergey Poluyan <svpoluyan@gmail.com>

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

#ifndef TEST_H
#define TEST_H

#include "print2file.h"
#include "timer.h"
#include "trie_based.h"
#include "quantile.h"

std::pair<size_t, float> ecdf1d_pair(const std::vector<float> &sample, const std::vector<float> &grid, float val01);
void ecdfNd_one_MultipleGrids(const std::vector<std::vector<float> > &sample, const std::vector<std::vector<float> > &grids, const std::vector<float> &val01, std::vector<float> &rez);
void explicit_quantile(std::vector<std::vector<float> > &sample, std::vector<std::vector<float> > &grids, size_t nrolls);

void implicit_quantile_class(float lb, float ub, std::vector<size_t> gridn, std::vector<std::vector<int> > &sample, size_t nrolls);

void test_1d1();
void test_1d2();

void test_2d1();

void test_3d1();
void test_3d2();

#endif