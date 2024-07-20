#version 120

uniform sampler2D texture;
// uniform vec3 devisions; // where the color devisions should be

// Brown
// 173, 103, 078
// 230, 176, 138
// 245, 204, 164
// 255, 229, 181

// Blue
// 079.0, 125.0, 171.0
// 124.0, 189.0, 222.0
// 161.0, 225.0, 237.0
// 184.0, 248.0, 255.0

// Green
// 080.0, 171.0, 095.0
// 145.0, 222.0, 124.0
// 198.0, 237.0, 175.0
// 231.0, 255.0, 204.0

void main()
{
  // lookup the pixel in the texture
  // vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

  // gl_FragColor = gl_Color * pixel;

  // if (gl_FragColor.a <= 0.0)
  // {
  //   return;
  // }

  // gl_FragColor /= 255.0;
  // gl_FragColor.a = 1.0;

  vec2 fc = vec2(gl_FragCoord.x - 480.0, gl_FragCoord.y - 270.0);

  // vec4 score = vec4(100.0, 200.0, 500.0, 100.0);
  vec4 score = vec4(100.0, 190.0, -100.0, 0.0);

  float total = score.x + score.y + score.z + score.w;
  vec4 ratio = score/total * 960.0;
  // +- 384, 192


  vec4 c = texture2D(texture, gl_TexCoord[0].xy);

  if (c.a == 0.0)
  {
    return;
  }

  float pixel = c.r;

  if (pixel == 0.0)
  {
    if (fc.x <= 0 && fc.x > -366.0 && abs(fc.y) < 192.0)
    {
      pixel = 0.8;
    }
    else if (fc.x > 0 && fc.x < 384.0 && abs(fc.y) < 192.0)
    {
      pixel = 0.8;
    }

    else if (fc.x <= 0 && fc.x > -384.0 && fc.x < -372.0f && abs(fc.y) < 192.0)
    {
      pixel = 0.6;
    }

    else if (fc.x <= 0 && fc.x > -390.0 && abs(fc.y) < 198.0)
    {
      pixel = 0.2;
    }
    else if (fc.x > 0 && fc.x < 390.0 && abs(fc.y) < 198.0)
    {
      pixel = 0.2;
    }

    else
    {
      pixel = 0.4;
    }
  }

  // vec3 col = vec3(1);
  int colorID = 0;
  
  float temp = ratio.x;   
  float ytemp = (gl_FragCoord.y - 180.0) / 32.0;
  if (ytemp >= 0.0)
  {
      ytemp += 1.0;
  }
  int test = int(gl_FragCoord.x / 8.0) + int(ytemp);
  if(float(test) * 8.0 < temp)
  {
    colorID = 0;
  }
  else if(float(test) * 8.0 < temp + ratio.y)
  {
    colorID = 1;
  }
  else if(float(test) * 8.0 < temp + ratio.y + ratio.z)
  {
    colorID = 2;
  }   
  else
  {
    colorID = 3;
  }

  // Uncomment this to have a monochrome background
  colorID = 0;

  if (colorID == 0)
  {
    if (pixel <= 0.2) // darker
    {
      gl_FragColor = vec4(173.0, 103.0, 78.0, 1.0);
    }
    else if (pixel <= 0.4) // dark
    {
      gl_FragColor = vec4(230.0, 176.0, 138.0, 1.0);
    }
    else if (pixel <= 0.6) // light
    {
      gl_FragColor = vec4(245.0, 204.0, 164.0, 1.0);
    }
    else if (pixel <= 0.8) // lighter
    {
      gl_FragColor = vec4(255.0, 229.0, 181.0, 1.0);
    }
  }

  else if (colorID == 1)
  {
    if (pixel <= 0.2) // darker
    {
      gl_FragColor = vec4(079.0, 125.0, 171.0, 1.0);
    }
    else if (pixel <= 0.4) // dark
    {
      gl_FragColor = vec4(124.0, 189.0, 222.0, 1.0);
    }
    else if (pixel <= 0.6) // light
    {
      gl_FragColor = vec4(161.0, 225.0, 237.0, 1.0);
    }
    else if (pixel <= 0.8) // lighter
    {
      gl_FragColor = vec4(184.0, 248.0, 255.0, 1.0);
    }
  }

  else if (colorID == 2)
  {
    if (pixel <= 0.2) // darker
    {
      gl_FragColor = vec4(148.0, 080.0, 171.0, 1.0);
    }
    else if (pixel <= 0.4) // dark
    {
      gl_FragColor = vec4(210.0, 124.0, 222.0, 1.0);
    }
    else if (pixel <= 0.6) // light
    {
      gl_FragColor = vec4(237.0, 178.0, 226.0, 1.0);
    }
    else if (pixel <= 0.8) // lighter
    {
      gl_FragColor = vec4(255.0, 204.0, 241.0, 1.0);
    }
  }

  else if (colorID == 3)
  {
    if (pixel <= 0.2) // darker
    {
      gl_FragColor = vec4(080.0, 171.0, 095.0, 1.0);
    }
    else if (pixel <= 0.4) // dark
    {
      gl_FragColor = vec4(145.0, 222.0, 124.0, 1.0);
    }
    else if (pixel <= 0.6) // light
    {
      gl_FragColor = vec4(198.0, 237.0, 175.0, 1.0);
    }
    else if (pixel <= 0.8) // lighter
    {
      gl_FragColor = vec4(231.0, 255.0, 204.0, 1.0);
    }
  }

  gl_FragColor /= 255.0;
  gl_FragColor.a = 1.0;
  
  // Output to screen
  // gl_FragColor = vec4(col,1.0);

}