@vs vs
in vec4 pos;
void main() {
    gl_Position = pos;
}
@end

@fs fs
out vec4 frag_color;
void main() {
    frag_color = vec4(1.0, 0.0, 0.0, 1.0);
}
@end

@program samogonki vs fs