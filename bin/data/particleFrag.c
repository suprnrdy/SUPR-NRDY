#version 120
#extension GL_ARB_texture_rectangle : enable
#extension GL_EXT_gpu_shader4 : enable

uniform sampler2DRect texture0;

void main(){
	//Getting coordinates of the current pixel in texture
	vec2 pos = gl_TexCoord[0].xy;
    
	//Getting the pixel color from the texture texture0 in pos
	vec4 color = texture2DRect(texture0, pos);
	
	//Output the color of shader
	gl_FragColor = color;
}
