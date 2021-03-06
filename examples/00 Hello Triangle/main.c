#include "../../gpulib.h"

typedef struct
{
  float x, y, z;
} vec3;

int32_t main()
{
  SDL_Window * sdl_window = NULL;
  gpu_window("Hello Triangle", 1280, 720, 4, 0, 0, &sdl_window, NULL);

  vec3 * mesh = gpu_malloc(3 * sizeof(vec3));

  mesh[0].x = 0.0f;
  mesh[0].y = 0.5f;
  mesh[0].z = 0.0f;

  mesh[1].x = -0.5f;
  mesh[1].y = -0.5f;
  mesh[1].z = 0.0f;

  mesh[2].x = 0.5f;
  mesh[2].y = -0.5f;
  mesh[2].z = 0.0f;

  uint32_t vert = gpu_vert(
      gpu_vert_head " layout(binding = 0) uniform samplerBuffer s_pos; \n"
                    "                                                  \n"
                    " void main()                                      \n"
                    " {                                                \n"
                    "   vec3 pos = texelFetch(s_pos, gl_VertexID).xyz; \n"
                    "   gl_Position = vec4(pos, 1.f);                  \n"
                    " }                                                \n");

  uint32_t frag = gpu_frag(
      gpu_frag_head " out vec4 color;      \n"
                    "                      \n"
                    " void main()          \n"
                    " {                    \n"
                    "   color = vec4(1.f); \n"
                    " }                    \n");

  uint32_t ppo = gpu_ppo(vert, frag);

  uint32_t tex = gpu_cast(mesh, gpu_xyz_f32_t, 0, 3 * sizeof(vec3));

  uint32_t textures[16] = {};
  textures[0] = tex;

  struct gpu_cmd_t cmd[1] = {};
  cmd[0].first = 0;
  cmd[0].count = 3;
  cmd[0].instance_first = 0;
  cmd[0].instance_count = 1;

  struct gpu_ops_t ops[1] = {};
  ops[0].id = 0;
  ops[0].tex_first = 0;
  ops[0].tex_count = 16;
  ops[0].tex = textures;
  ops[0].smp_first = 0;
  ops[0].smp_count = 0;
  ops[0].smp = NULL;
  ops[0].vert = 0;
  ops[0].frag = 0;
  ops[0].ppo = ppo;
  ops[0].mode = gpu_triangles_t;
  ops[0].cmd_count = 1;
  ops[0].cmd = cmd;

  while (1)
  {
    gpu_clear();
    gpu_draw(1, ops);
    gpu_swap(sdl_window);

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        goto exit;
    }
  }

exit:
  return 0;
}
