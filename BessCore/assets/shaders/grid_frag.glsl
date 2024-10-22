#version 460 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out int fragColor1;

in vec3 v_FragPos;
in vec2 v_TexCoord;
in flat int v_FragId;
in vec4 v_FragColor;
in float v_AR;

float grid_intensity = 0.5f;

uniform float u_zoom;
uniform vec2 u_cameraOffset;

float grid(vec2 fragCoord, float space, float gridWidth)
{
    vec2 p  = fragCoord - vec2(.5);
    vec2 size = vec2(gridWidth);
    vec2 a1 = mod(p - size, space);
    vec2 a2 = mod(p + size, space);
    vec2 a = a2 - a1;
    float g = max(a.x, a.y);
    return clamp(g, 0., 1.0);
}

void main() {
    float ar = v_AR;
    vec2 fragCoord = gl_FragCoord.xy;
    float scale = u_zoom;
    fragCoord /= scale;
    fragCoord -= u_cameraOffset;

    float gap = 25.f * scale;
    float dotSize = 2.f;
    vec2 gridPos = mod(fragCoord, gap);
    float dotRadius = 2.f;

    vec2 dotCenter = vec2(gap / 2.0);

    float dist = distance(gridPos, dotCenter);
    float a = smoothstep(dotRadius, dotRadius - 1.f, dist);
    if(a == 0.f) discard;
    vec4 col = v_FragColor * a;

    fragColor = col;
    fragColor1 = v_FragId;
}
