#version 460 core

layout(location = 0) out vec4 fragColor;
layout(location = 1) out int fragColor1;

in vec2 v_Size;
in vec2 v_TexCoord;
in vec4 v_FragColor;
in vec4 v_BorderColor;
in vec4 v_BorderSize;
in vec4 v_BorderRadius;
in flat int v_FragId;

uniform int u_SelectedObjId;
uniform float u_zoom;

float sdRoundedBox( in vec2 p, in vec2 b, in vec4 r )
{
    r.xy = (p.x>0.0)?r.xy : r.zw;
    r.x  = (p.y>0.0)?r.x  : r.y;
    vec2 q = abs(p)-b+r.x;
    return min(max(q.x,q.y),0.0) + length(max(q,0.0)) - r.x;
}
void main()
{
    vec2 si = v_Size;
    vec4 br = v_BorderRadius;
    vec4 bt = v_BorderSize;
    vec2 si_ = si - vec2(bt.x + bt.w, bt.y + bt.z);
    vec3 col = v_FragColor.xyz;
    vec3 bCol = v_BorderColor.xyz;
        
    vec2 uv = (2.0*v_TexCoord) - 1.f;
    float nf = min(si.x, si.y);
    si /= nf;    
    si_ /= nf;
    br /= nf;    
    bt /= nf;

    float oX  = bt.x - ((bt.x + bt.w) / 2.f);
    float oY =  bt.y - ((bt.y + bt.z) / 2.f);
    vec2 offset = vec2(-oX, oY);

    float a = sdRoundedBox(uv, si, br);
    float a1 = sdRoundedBox(uv + offset , si_, br - bt);
    
    float blur = 0.035f;
    a = smoothstep(blur, 0.0, a);

    if(a == 0.f) discard;

    a1 = smoothstep(blur, 0.0, a1);
    vec3 col1 = bCol * a;
    col = mix(col1, col, a1);
    fragColor = vec4(col, min(a, v_BorderColor.w));
    fragColor1 = v_FragId;
}
