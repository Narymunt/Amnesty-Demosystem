vs.1.0
        
; r0: camera-space position
; r1: camera-space normal
; r2: camera-space vertex-eye vector
; r3: camera-space reflection vector
; r4: texture coordinates

; Transform position and normal into camera-space
m4x4 r0, v0, c0
m3x3 r1, v3, c0

; Compute normalized view vector
add r2, c8, -r0
dp3 r3, r2, r2
rsq r3, r3
mul r2, r2, r1

; Compute camera-space reflection vector
dp3 r3, r2, r1
mul r1, r1, r3
add r1, r3, r1
add r3,-r1, r2				; r3 - reflection vector

; Compute sphere-map texture coords
add r3.z, r3.z, c9.z		; Rz+1
dp3 r4, r3, r3				; Rx^2+Ry^2+(Rz+1)^2
rsq r5, r4					; 1/sqrt(Rx^2+Ry^2+(Rz+1)^2)
mul r5, r5, c9.y

; Project position
m4x4 oPos, r0, c4
mov oT1, v7
mad oT0.xy, r3.xy, r5.xy, c9.y
mov oD0, v5