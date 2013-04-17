#version 110

uniform float fade_factor;
uniform sampler2D textures;

varying vec2 texcoord;

void main()
{
  // vec4 texval1 = texture2D(textures, vec2(gl_TexCoord[0]));

 // gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);

 gl_FragColor = texture2D(textures, gl_TexCoord[0].st)*fade_factor;
}