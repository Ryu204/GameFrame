#include "VertexArray.hpp"

namespace HJUIK
{
  namespace Graphics
  {
    auto VertexArray::bind() const -> void { glBindVertexArray(get()); }
    auto VertexArray::unbind() -> void { glBindVertexArray(0); }
  } // namespace Graphics

} // namespace HJUIK
