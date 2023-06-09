/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org

Copyright (c) 2000-2014 Torus Knot Software Ltd
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
-----------------------------------------------------------------------------
*/

//-----------------------------------------------------------------------------
// Program Name: FFPLib_Transform
// Program Desc: Transform functions of the FFP.
// Program Type: Vertex shader
// Language: GLSL
// Notes: Implements core functions for FFPTransform class.
// based on transform engine. 
// See http://msdn.microsoft.com/en-us/library/bb206269.aspx
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
void FFP_Transform(in mat4 m, 
				   in vec4 v, 
				   out vec3 vOut)
{
	vOut = mul(m, v).xyz;
}

#ifdef OGRE_HLSL
void FFP_Transform(in float3x4 m,
				   in float4 v,
				   out float3 vOut)
{
	vOut = mul(m, v);
}

//-----------------------------------------------------------------------------
void FFP_Transform(in float3x4 m,
				   in float3 v,
				   out float3 vOut)
{
	vOut = mul((float3x3)m, v);
}
#elif !defined(OGRE_GLSLES) || OGRE_GLSLES > 100
//-----------------------------------------------------------------------------
void FFP_Transform(in mat3x4 m,
				   in vec4 v,
				   out vec3 vOut)
{
/* transpose non-square uniform matrix for correct row-major > column-major mapping
 * to keep the indexing inside the shader so mat[0] returns the same data in both GLSL and HLSL
 * although it will be the first row in HLSL and the first column in GLSL
 */
	vOut = v * m;
}

void FFP_Transform(in mat3x4 m,
				   in vec3 v,
				   out vec3 vOut)
{
	vOut = v * mat3(m);
}
#endif

//-----------------------------------------------------------------------------
void FFP_Transform(in mat4 m, 
				   in vec3 v, 
				   out vec3 vOut)
{
#ifdef OGRE_HLSL
	vOut = mul((float3x3)m, v);
#else
	vOut = mat3(m) * v;
#endif
}

//-----------------------------------------------------------------------------
void FFP_DerivePointSize(in vec4 params,
                         in float d,
                         out float sz)
{
	sz = params.x/sqrt(params.y + params.z*d + params.w*d*d);
}
