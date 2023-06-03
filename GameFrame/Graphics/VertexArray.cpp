#include "VertexArray.hpp"

namespace HJUIK
{
  namespace Graphics
  {
    auto VertexArray::bind() -> void const { glBindVertexArray(get()); }
    auto VertexArray::unbind() -> void const { glBindVertexArray(0); }
  } // namespace Graphics

} // namespace HJUIK
