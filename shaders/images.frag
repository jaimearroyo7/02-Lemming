#version 330

uniform vec4 color;
uniform sampler2D tex;
uniform float alpha;

in vec2 texCoordFrag;
out vec4 outColor;

void main()
{
	vec4 texColor = texture(tex, texCoordFrag);
	if(texColor.a < 0.5f)
		discard;
	outColor = color * texColor;
	outColor.a = alpha;
}

