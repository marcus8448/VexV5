#ifndef STRUCTURE_CONCEPT_HPP
#define STRUCTURE_CONCEPT_HPP
#include <concepts>

template<typename T> concept Numeric = (std::integral<T> || std::floating_point<T>);

#endif //STRUCTURE_CONCEPT_HPP
