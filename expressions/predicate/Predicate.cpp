/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 **/

#include "expressions/predicate/Predicate.hpp"

#include <string>
#include <vector>

#include "storage/TupleIdSequence.hpp"
#include "storage/ValueAccessor.hpp"
#include "utility/Macros.hpp"

namespace quickstep {

const char *Predicate::kPredicateTypeNames[] = {
  "True",
  "False",
  "Comparison",
  "Negation",
  "Conjunction",
  "Disjunction"
};

bool Predicate::getStaticResult() const {
  FATAL_ERROR("Called getStaticResult() on a predicate which has no static result");
}

TupleIdSequence* Predicate::GenerateSequenceForStaticResult(
    ValueAccessor *accessor,
    const TupleIdSequence *filter,
    const TupleIdSequence *existence_map,
    const bool static_result) {
  TupleIdSequence *result = new TupleIdSequence(accessor->getEndPositionVirtual());
  if (static_result) {
    if (filter != nullptr) {
      // '*filter' will always be a subset of '*existence_map'.
      result->assignFrom(*filter);
    } else if (existence_map != nullptr) {
      result->assignFrom(*existence_map);
    } else {
      result->setRange(0, result->length(), true);
    }
  }
  return result;
}

void Predicate::getFieldStringItems(
    std::vector<std::string> *inline_field_names,
    std::vector<std::string> *inline_field_values,
    std::vector<std::string> *non_container_child_field_names,
    std::vector<const Expression*> *non_container_child_fields,
    std::vector<std::string> *container_child_field_names,
    std::vector<std::vector<const Expression*>> *container_child_fields) const {
  if (hasStaticResult()) {
    inline_field_names->emplace_back("static_result");
    inline_field_values->emplace_back(getStaticResult() ? "true" : "false");
  }
}

}  // namespace quickstep
