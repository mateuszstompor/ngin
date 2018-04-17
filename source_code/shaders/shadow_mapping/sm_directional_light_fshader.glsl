R"(

void main() {
//    float near = 0.001f;
//    float far = 100.0f;
//    float z = gl_FragCoord.z * 2.0 - 1.0;
//    float a = (2.0 * near* far) / (far + near - z * (far - near));
//    gl_FragDepth = a / far;
}

// TODO linearize depth
// pass far and near as params

)";
