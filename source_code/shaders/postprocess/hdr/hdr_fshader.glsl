R"(

//adaptive tone mapping
//25 samples

in vec2 texCoords;

layout (location = 0) out vec4 out1;

uniform sampler2D in0;

void main() {

	//stores samples
	float bf[25];
	vec2 texelScale = vec2(1.0) / vec2(textureSize(in0, 0));
	
	for (int i = 0; i < 25; ++i) {
		vec2 tc = (gl_FragCoord.xy + vec2(i % 5 - 2, i / 5 - 2));
		vec3 col = texture(in0, tc * texelScale).rgb;
		bf[i] = get_luminance_for_adaptive_tone(col);
	}
	
	float weightsSum = 273.0f; // 4 * 26.0 + 4 * 16.0 + 4 * 7.0 + 8 * 4.0 + 4 * 1.0 + 1 * 41.0
	
	vec3 color = texelFetch(in0, ivec2(gl_FragCoord.xy), 0).rgb;
	float weightedAvarageLuminance = ((1.0 * (bf[0] + bf[4] + bf[20] + bf[24])) +
									  (4.0 * (bf[1] + bf[3] + bf[5] + bf[9])) +
									  (4.0 * (bf[15] + bf[19] + bf[21] + bf[23])) +
									  (7.0 * (bf[2] + bf[10] + bf[14] + bf[22])) +
									  (16.0 * (bf[6] + bf[8] + bf[16] + bf[18])) +
									  (26.0 * (bf[7] + bf[11] + bf[13] + bf[17])) +
									  (41.0 * bf[12])) / weightsSum;

	float exposure = sqrt(8.0f / (weightedAvarageLuminance + 0.25f));
	out1.rgb = color_with_exposure(color, exposure);
	out1.a = 1.0f;

}

)";
