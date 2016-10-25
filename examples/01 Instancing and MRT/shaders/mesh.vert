#version 330
// clang-format off
#extension GL_ARB_gpu_shader5               : enable
#extension GL_ARB_gpu_shader_fp64           : enable
#extension GL_ARB_shader_precision          : enable
#extension GL_ARB_conservative_depth        : enable
#extension GL_ARB_texture_cube_map_array    : enable
#extension GL_ARB_separate_shader_objects   : enable
#extension GL_ARB_shading_language_420pack  : enable
#extension GL_ARB_shading_language_packing  : enable
#extension GL_ARB_explicit_uniform_location : enable
// clang-format on
out gl_PerVertex { vec4 gl_Position; };

vec4 proj(vec3 mv, vec4 p)
{
  return vec4(mv.xy * p.xy, fma(mv.z, p.z, p.w), -mv.z);
}
vec3 qrot(vec3 v, vec4 q)
{
  return fma(cross(q.xyz, fma(v, vec3(q.w), cross(q.xyz, v))), vec3(2.0), v);
}
vec4 qconj(vec4 q) { return vec4(-q.xyz, q.w); }

layout(binding = 0) uniform samplerBuffer s_mesh;
layout(binding = 1) uniform samplerBuffer s_pos;

layout(location = 0) uniform int id;
layout(location = 1) uniform float fcoef;
layout(location = 2) uniform vec3 cam_pos;
layout(location = 3) uniform vec4 cam_rot;
layout(location = 4) uniform vec4 cam_prj;

layout(location = 0) smooth out float flogz;
layout(location = 1) smooth out vec3 pos;
layout(location = 2) smooth out vec3 nor;
layout(location = 3) smooth out vec2 uv;

void main()
{
  vec4 mesh_1 = texelFetch(s_mesh, gl_VertexID * 2 + 0);
  vec4 mesh_2 = texelFetch(s_mesh, gl_VertexID * 2 + 1);

  pos = mesh_1.xyz;
  nor = mesh_2.xyz;
  uv = vec2(mesh_1.w, mesh_2.w);

  pos += texelFetch(s_pos, gl_InstanceID).xyz;

  vec3 mv = pos;
  mv -= cam_pos;
  mv = qrot(mv, qconj(cam_rot));
  gl_Position = proj(mv, cam_prj);

  gl_Position.z = log2(max(1e-6, 1.0 + gl_Position.w)) * fcoef - 1.0;
  flogz = 1.0 + gl_Position.w;
}
