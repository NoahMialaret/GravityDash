#version 120

uniform int colorID;    // Determines which colour to render the texture in
uniform int scale;      // The scale objects are rendered at
uniform vec2 worldDim;  // The dimensions of the world without scaling

mat4 brown = mat4(
  vec4(255.0, 229.0, 181.0, 255.0),
  vec4(245.0, 204.0, 164.0, 255.0),
  vec4(230.0, 176.0, 138.0, 255.0),
  vec4(173.0, 103.0, 78.0, 255.0)
);

mat4 green = mat4(
  vec4(231.0, 255.0, 204.0, 255.0),
  vec4(198.0, 237.0, 175.0, 255.0),
  vec4(145.0, 222.0, 124.0, 255.0),
  vec4(80.0, 171.0, 96.0, 255.0)
);

mat4 blue = mat4(
  vec4(184.0, 248.0, 255.0, 255.0),
  vec4(161.0, 225.0, 237.0, 255.0),
  vec4(124.0, 189.0, 222.0, 255.0),
  vec4(79.0, 125.0, 171.0, 255.0)
);

mat4 purple = mat4(
  vec4(255.0, 204.0, 241.0, 255.0),
  vec4(237.0, 178.0, 226.0, 255.0),
  vec4(210.0, 124.0, 222.0, 255.0),
  vec4(148.0, 80.0, 171.0, 255.0)
);

void main()
{
  vec2 screenDim = vec2(160.0 * scale, 90.0 * scale);
  vec2 scaledWorldDim = worldDim.xy * scale;
  vec2 fc = gl_FragCoord.xy - screenDim.xy / 2;

  mat4 col;
  if (colorID == 0)
    col = brown;
  if (colorID == 1)
    col = green;
  if (colorID == 2)
    col = blue;
  if (colorID == 3)
    col = purple;

  // Outside  
  if (abs(fc.x) >= scaledWorldDim.x + scale 
      || abs(fc.y) >= scaledWorldDim.y + scale)
    gl_FragColor = col[2];
  
  // Border
  else if (abs(fc.x) >= scaledWorldDim.x 
      || abs(fc.y) >= scaledWorldDim.y)
    gl_FragColor = col[3];
  
  else if (fc.x <= - scaledWorldDim.x + 2 * scale)
    gl_FragColor = col[1];

  else if (fc.x <= - scaledWorldDim.x + 3 * scale)
    gl_FragColor = col[3];

  else
    gl_FragColor = col[0];


  // if (pixel == 0.0)
  // {
  //   if (fc.x <= 0 && fc.x > -366.0 && abs(fc.y) < 192.0)
  //   {
  //     pixel = 0.8;
  //   }
  //   else if (fc.x > 0 && fc.x < 384.0 && abs(fc.y) < 192.0)
  //   {
  //     pixel = 0.8;
  //   }

  //   else if (fc.x <= 0 && fc.x > -384.0 && fc.x < -372.0f && abs(fc.y) < 192.0)
  //   {
  //     pixel = 0.6;
  //   }

  //   else if (fc.x <= 0 && fc.x > -390.0 && abs(fc.y) < 198.0)
  //   {
  //     pixel = 0.2;
  //   }
  //   else if (fc.x > 0 && fc.x < 390.0 && abs(fc.y) < 198.0)
  //   {
  //     pixel = 0.2;
  //   }

  //   else
  //   {
  //     pixel = 0.4;
  //   }
  // }

  gl_FragColor /= 255.0;
}