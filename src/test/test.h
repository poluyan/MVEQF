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

#include <trie_based.h>
#include <quantile.h>

void explicit_quantile(float lb, float ub, std::vector<size_t> gridn, std::vector<std::vector<int> > &sample, size_t nrolls);
void implicit_quantile_class(float lb, float ub, std::vector<size_t> gridn, std::vector<std::vector<int> > &sample, size_t nrolls);
void implicit_quantile_class_sorted(float lb, float ub, std::vector<size_t> gridn, std::vector<std::vector<int> > &sample, size_t nrolls);
void implicit_quantile_class_sorted_interp(float lb, float ub, std::vector<size_t> gridn, std::vector<std::vector<int> > &sample, size_t nrolls);
void implicit_quantile_graph_sorted(float lb, float ub, std::vector<size_t> gridn, size_t nrolls);

#endif