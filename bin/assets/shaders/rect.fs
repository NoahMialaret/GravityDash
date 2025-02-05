#version 120

// Shader used for rendering shapes

uniform int colorID;  // Determines which colour to render the pixel in

mat4 brown = mat4(
  vec4(255.0, 229.0, 181.0, 255.0),
  vec4(245.0, 204.0, 164.0, 255.0),
  vec4(230.0, 176.0, 138.0, 255.0),
  vec4(173.0, 103.0, 078.0, 255.0)
);

mat4 green = mat4(
  vec4(231.0, 255.0, 204.0, 255.0),
  vec4(198.0, 237.0, 175.0, 255.0),
  vec4(145.0, 222.0, 124.0, 255.0),
  vec4(080.0, 171.0, 096.0, 255.0)
);

mat4 blue = mat4(
  vec4(184.0, 248.0, 255.0, 255.0),
  vec4(161.0, 225.0, 237.0, 255.0),
  vec4(124.0, 189.0, 222.0, 255.0),
  vec4(079.0, 125.0, 171.0, 255.0)
);

mat4 purple = mat4(
  vec4(255.0, 204.0, 241.0, 255.0),
  vec4(237.0, 178.0, 226.0, 255.0),
  vec4(210.0, 124.0, 222.0, 255.0),
  vec4(148.0, 080.0, 171.0, 255.0)
);

void main()
{
  gl_FragColor = gl_Color;

  if (gl_FragColor.a <= 0.0)
    return;

  mat4 col;
  if (colorID == 0)
    col = brown;
  if (colorID == 1)
    col = green;
  if (colorID == 2)
    col = blue;
  if (colorID == 3)
    col = purple;

  for (int i = 0; i < 4; i++)
  {
    if (gl_FragColor.r <= 0.2 + i * 0.2)
    {
      gl_FragColor = col[3 - i];
      break;
    }
  }

  gl_FragColor /= 255.0;
}