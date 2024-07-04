#version 120

uniform sampler2D texture;
uniform int colorID; // Determines which colour to render the player in

// Brown
// 255, 229, 181
// 245, 204, 164
// 230, 176, 138
// 173, 103, 078

// Blue
// 079.0, 125.0, 171.0
// 124.0, 189.0, 222.0
// 161.0, 225.0, 237.0
// 184.0, 248.0, 255.0

// Purp
// 148.0, 080.0, 171.0
// 210.0, 124.0, 222.0
// 237.0, 178.0, 226.0
// 255.0, 204.0, 241.0


void main()
{
  // lookup the pixel in the texture
  vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

  // if (gl_FragCoord.y < 400.0)
  // {
  //   gl_FragColor = vec4(0.0);
  //   return;
  // }

  vec2 fc = vec2(gl_FragCoord.x - 480.0, gl_FragCoord.y - 270.0);
  if (abs(fc.x) >= 384.0 || abs(fc.y) >= 192.0)
  {
    // gl_FragColor = vec4(1.0);
    return;
  }

  gl_FragColor = gl_Color * pixel;

  if (gl_FragColor.a <= 0.0)
  {
    return;
  }

  int colorID = 0;

  if (colorID == 0)
  {
    if (pixel.r <= 0.2) // darker
    {
      gl_FragColor = vec4(173.0, 103.0, 78.0, 1.0);
    }
    else if (pixel.r <= 0.4) // dark
    {
      gl_FragColor = vec4(230.0, 176.0, 138.0, 1.0);
    }
    else if (pixel.r <= 0.6) // light
    {
      gl_FragColor = vec4(245.0, 204.0, 164.0, 1.0);
    }
    else if (pixel.r <= 0.8) // lighter
    {
      gl_FragColor = vec4(255.0, 229.0, 181.0, 1.0);
    }
  }

  else if (colorID == 1)
  {
    if (pixel.r <= 0.2) // darker
    {
      gl_FragColor = vec4(079.0, 125.0, 171.0, 1.0);
    }
    else if (pixel.r <= 0.4) // dark
    {
      gl_FragColor = vec4(124.0, 189.0, 222.0, 1.0);
    }
    else if (pixel.r <= 0.6) // light
    {
      gl_FragColor = vec4(161.0, 225.0, 237.0, 1.0);
    }
    else if (pixel.r <= 0.8) // lighter
    {
      gl_FragColor = vec4(184.0, 248.0, 255.0, 1.0);
    }
  }

  else if (colorID == 2)
  {
    if (pixel.r <= 0.2) // darker
    {
      gl_FragColor = vec4(148.0, 080.0, 171.0, 1.0);
    }
    else if (pixel.r <= 0.4) // dark
    {
      gl_FragColor = vec4(210.0, 124.0, 222.0, 1.0);
    }
    else if (pixel.r <= 0.6) // light
    {
      gl_FragColor = vec4(237.0, 178.0, 226.0, 1.0);
    }
    else if (pixel.r <= 0.8) // lighter
    {
      gl_FragColor = vec4(255.0, 204.0, 241.0, 1.0);
    }
  }

  else if (colorID == 3)
  {
    if (pixel.r <= 0.2) // darker
    {
      gl_FragColor = vec4(080.0, 171.0, 095.0, 1.0);
    }
    else if (pixel.r <= 0.4) // dark
    {
      gl_FragColor = vec4(145.0, 222.0, 124.0, 1.0);
    }
    else if (pixel.r <= 0.6) // light
    {
      gl_FragColor = vec4(198.0, 237.0, 175.0, 1.0);
    }
    else if (pixel.r <= 0.8) // lighter
    {
      gl_FragColor = vec4(231.0, 255.0, 204.0, 1.0);
    }
  }

  gl_FragColor /= 255.0;
  gl_FragColor.a = 1.0;

  // multiply it by the color
  // gl_FragColor.r = abs(gl_FragCoord.x / 800.0);
  // gl_FragColor.a = 1.0;
  // gl_FragColor = gl_Color * pixel;
}