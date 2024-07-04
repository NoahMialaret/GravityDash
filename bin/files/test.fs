uniform sampler2D texture;

in vec2 fragCoord;

void main()
{
  // lookup the pixel in the texture
  vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

  vec4 color = gl_Color;

  if (gl_FragCoord.x > 2000)
  {
    color = vec4(0.0);
  }

  // multiply it by the color
  gl_FragColor = color * pixel;

} 