R"(

in vec2 texCoords;

layout (location = 0) out vec4 fragment;

float innerCircleRadius = 0.25f;
float outerCircleRadius = 0.8f;
float effectIntensity = 0.7f;

uniform sampler2D in0;
uniform int isOn;


void main() {
    
    if (isOn == 1) {
        vec2 relativePosition = gl_FragCoord.xy / vec2(textureSize(in0, 0)) - 0.5f;
        float distanceToCenter = length2(relativePosition);
        vec4 sampledColor = texture(in0, texCoords);
        
        if(distanceToCenter > innerCircleRadius) {
            if (outerCircleRadius < distanceToCenter){
                fragment = vec4(0.0f, 0.0f, 0.0f, 1.0f);
            } else {
                //Hermite interpolation in order to achive smooth transition from one color to another
                float vignette = smoothstep(outerCircleRadius, innerCircleRadius, distanceToCenter);
                //mix takes average of two products x*(1−a)+y*a
                fragment = vec4(mix(sampledColor, sampledColor * vignette, effectIntensity));
            }
        } else {
            fragment = sampledColor;
        }
    } else {
        fragment = texture(in0, texCoords);
    }
    
}

)";

