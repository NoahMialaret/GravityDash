#version 120

// Shader used for rendering shapes

uniform int colorID;    // Determines which colour to render the pixel in
uniform vec2 screenDim; // The dimensions of the screen

mat4x3 brown = mat4x3(
  vec3(255.0, 229.0, 181.0),
  vec3(245.0, 204.0, 164.0),
  vec3(230.0, 176.0, 138.0),
  vec3(173.0, 103.0, 078.0)
);

mat4x3 green = mat4x3(
  vec3(231.0, 255.0, 204.0),
  vec3(198.0, 237.0, 175.0),
  vec3(145.0, 222.0, 124.0),
  vec3(080.0, 171.0, 096.0)
);

mat4x3 blue = mat4x3(
  vec3(184.0, 248.0, 255.0),
  vec3(161.0, 225.0, 237.0),
  vec3(124.0, 189.0, 222.0),
  vec3(079.0, 125.0, 171.0)
);

mat4x3 purple = mat4x3(
  vec3(255.0, 204.0, 241.0),
  vec3(237.0, 178.0, 226.0),
  vec3(210.0, 124.0, 222.0),
  vec3(148.0, 080.0, 171.0)
);

void main()
{
  vec2 fc = gl_FragCoord.xy - screenDim.xy / 2;
  if (abs(fc.x) >= screenDim.x / 2 
      || abs(fc.y) >= screenDim.y / 2)
    return;

  gl_FragColor = gl_Color;

  if (gl_FragColor.a <= 0.0)
    return;

  mat4x3 col;
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
      gl_FragColor.rgb = col[3 - i].rgb;
      break;
    }
  }

  gl_FragColor.rgb /= 255.0;
}