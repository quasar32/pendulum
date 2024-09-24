#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <time.h>

#define FPS 100
#define DT (1.0F / FPS) 
#define N_STEPS 100
#define SDT (DT / N_STEPS)
#define N_BEADS 5

typedef float f32;
typedef int32_t i32;

typedef struct vec2 {
  f32 x;
  f32 y;
} vec2;

static f32 gravity = -10.0F;

static vec2 vec2_sub(vec2 a, vec2 b) {
  return (vec2) {a.x - b.x, a.y - b.y};
}

static vec2 vec2_adds(vec2 a, vec2 b, float s) {
  return (vec2) {a.x + b.x * s, a.y + b.y * s};
}

static vec2 vec2_subs(vec2 a, vec2 b, float s) {
  return (vec2) {a.x - b.x * s, a.y - b.y * s};
}

static f32 vec2_dot(vec2 a, vec2 b) {
  return a.x * b.x + a.y * b.y;
}

static vec2 vec2_divs(vec2 a, float b) {
  return (vec2) {a.x / b, a.y / b};
}

static f32 vec2_len(vec2 a) {
  return sqrtf(vec2_dot(a, a));
}

#define N_SRC 3
#define N_DST 4

static f32 lengths[N_SRC] = {0.2f, 0.2f, 0.2f};
static f32 masses[N_SRC] = {1.0f, 0.5f, 0.3f};
static f32 angles[N_SRC] = {M_PI_2, M_PI, M_PI};

typedef struct pendulum {
  f32 masses[N_DST];
  f32 lengths[N_DST];
  vec2 pos[N_DST];
  vec2 prev_pos[N_DST];
  vec2 vel[N_DST];
} pendulum;

static pendulum pnd;

static void sim_init(void) {
  pnd.masses[0] = 0.0f;
  pnd.lengths[0] = 0.0f;
  pnd.pos[0] = (vec2) {};
  pnd.prev_pos[0] = (vec2) {};
  pnd.vel[0] = (vec2) {};
  vec2 v = {};
  for (i32 i = 0; i < N_SRC; i++) {
    pnd.masses[i + 1] = masses[i];
    pnd.lengths[i + 1] = lengths[i];
    v.x += lengths[i] * sinf(angles[i]);
    v.y -= lengths[i] * cosf(angles[i]);
    pnd.pos[i + 1] = v;
    pnd.prev_pos[i + 1] = v;
    pnd.vel[i + 1] = (vec2) {};
  }
}

static void sim_update(void) {
  for (i32 i = 1; i < N_DST; i++) {
    pnd.vel[i].y += SDT * gravity;
    pnd.prev_pos[i] = pnd.pos[i];
    pnd.pos[i] = vec2_adds(pnd.pos[i], pnd.vel[i], SDT);
  }
  for (i32 i = 1; i < N_DST; i++) {
    vec2 dv = vec2_sub(pnd.pos[i], pnd.pos[i - 1]);
    f32 ds = vec2_len(dv);
    f32 w0 = pnd.masses[i - 1] > 0.0f ? 1.0f / pnd.masses[i - 1] : 0.0f;
    f32 w1 = pnd.masses[i] > 0.0f ? 1.0f / pnd.masses[i] : 0.0f;
    f32 corr = (pnd.lengths[i] - ds) / (ds * (w0 + w1));
    pnd.pos[i - 1] = vec2_subs(pnd.pos[i - 1], dv, w0 * corr);
    pnd.pos[i] = vec2_adds(pnd.pos[i], dv, w1 * corr);
  }
  for (i32 i = 1; i < N_DST; i++) {
    pnd.vel[i]= vec2_sub(pnd.pos[i], pnd.prev_pos[i]);
    pnd.vel[i] = vec2_divs(pnd.vel[i], SDT);
  }
}

static void print_sim(int f) {
  for (i32 i = 0; i < N_DST; i++) 
    printf("%d,%f,%f,%f\n", f, pnd.pos[i].x, pnd.pos[i].y, pnd.masses[i]);
}

int main(int argc, char **argv) {
  srand48(time(NULL));
  switch (argc) {
  case 0:
  case 1:
    break;
  case 2:
    FILE *tmp;
    tmp = freopen(argv[1], "wb", stdout);
    if (!tmp) {
      perror("freopen");
      exit(1);
    }
    stdout = tmp;
    break;
  default:
    fputs("too many args\n", stderr);
    exit(1);
  }
  sim_init();
  printf("f,x,y,m\n");
  i32 f;
  for (f = 0; f < 10 * FPS; f++) {
    print_sim(f);
    for (i32 s = 0; s < N_STEPS; s++) 
      sim_update();
  }
  print_sim(f);
}
