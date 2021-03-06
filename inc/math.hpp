#ifndef __math_hpp__
#define __math_hpp__

#include <cmath>

namespace math
{
#ifdef MATH_DOUBLE_PRECISION
  typedef double real;
#else
  typedef float real;
#endif // DOUBLE_PRECISION

  typedef unsigned short ushort;
  typedef unsigned int   uint;
}

#include "math/CVector.hpp"  // 
#include "math/CMatrix.hpp"  //

namespace math
{
  typedef CVector3   vec3;
  typedef CVector4   vec4;
  
  typedef CMatrix4x4 mat4;
  
  vec3 cross(const vec3& lhs, const vec3& rhs)
  {
    vec3 res;
    
    return res;
  }
  
  real dot(const vec3& lhs, const vec3& rhs)
  {
    real res = 0.0f;
    
    return res;
  }

  real length(const vec3& v)
  {
    return std::sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
  }
  
  vec3 normalize(const vec3& v)
  {
    real l = length(v);
    if(l != 1.0f)
      return vec3(v.x/l, v.y/l, v.z/l);
    return vec3(v);
  }
}

#include "math/IShape.hpp"    // 
#include "math/CPlane.hpp"    // requires CVector.hpp
#include "math/CTriangle.hpp" // requires CVector.hpp
#include "math/CFrustum.hpp"  // requires CPlane.hpp

#include "math/CShape.hpp"    // requires CVector.hpp
#include "math/CCube.hpp"     // requires CShape.hpp
#include "math/CCone.hpp"     // requires CShape.hpp
#include "math/CSphere.hpp"   // requires CShape.hpp
#include "math/CBox.hpp"      // requires CCube.hpp

namespace math
{
  /**
   * Split a CBox into 8 equal segments
   * @param  CBox& oAABB
   * @return std::array<CBox, 8>
   */
  std::array<CBox, 8> osplit(const CBox& oAABB)
  {
    std::array<CBox, 8> aAABB;
    aAABB.fill(CBox(0.0f));
    
    math::real fWidth  = oAABB.mWidth/2.0f;
    math::real fHeight = oAABB.mHeight/2.0f;
    math::real fDepth  = oAABB.mDepth/2.0f;
    
    for(size_t i = 0; i < 2; ++i) // x
    for(size_t j = 0; j < 2; ++j) // y
    for(size_t k = 0; k < 2; ++k) // z
    {
      size_t key = i * 2 * 2 + j * 2 + k;
      CBox& oFrag = aAABB[key];
      oFrag.mWidth  = fWidth;
      oFrag.mHeight = fHeight;
      oFrag.mDepth  = fDepth;
      oFrag.mPosition.x = oAABB.mPosition.x + (i == 0 ? (-fWidth)  : (+fWidth))/2.0f;
      oFrag.mPosition.y = oAABB.mPosition.y + (j == 0 ? (-fHeight) : (+fHeight))/2.0f;
      oFrag.mPosition.z = oAABB.mPosition.z + (k == 0 ? (-fDepth)  : (+fDepth))/2.0f;
    }
    
    return aAABB;
  }
  
  bool inside(const math::vec3& oPoint, const CBox& oBox)
  {
    math::real fHalfWidth  = oBox.mWidth/2.0f;
    math::real fHalfHeight = oBox.mHeight/2.0f;
    math::real fHalfDepth  = oBox.mDepth/2.0f;
    return (oPoint.x <= oBox.mPosition.x + fHalfWidth  && oBox.mPosition.x - fHalfWidth  <= oPoint.x) &&
           (oPoint.y <= oBox.mPosition.y + fHalfHeight && oBox.mPosition.y - fHalfHeight <= oPoint.y) &&
           (oPoint.z <= oBox.mPosition.z + fHalfDepth  && oBox.mPosition.z - fHalfDepth  <= oPoint.z);
  }
  
  struct compare
  {
    enum result { OUTSIDE, COLLIDE, INSIDE, EQUAL };
    
    result operator () (const CBox& smaller, const CBox& bigger)
    {
      // std::cout << smaller << " vs " << bigger << std::endl;
      
      if(smaller == bigger)  // equal
        return EQUAL;
        
      // @todo: check if bigger inside smaller => COLLIDE
      
      std::array<math::vec3, 8> aSVertices = smaller.getVertices();
      
      int status = 0;
      
      for(size_t i = 0; i < 8; ++i)
      {
        math::vec3& vertex = aSVertices[i];
        if(inside(vertex, bigger))
        {
          status |= 1 << i;
        }
      }
      
      if(status == 0)        // none
        return OUTSIDE;
      else if(status == 255) // all
        return INSIDE;
      else                   // partial
        return COLLIDE;
    }
  } compare;
}

#endif // __math_hpp__
