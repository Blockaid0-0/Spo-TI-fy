:DCS6
0FF01008200440028FC19031800987818861801183C184214002200410080FF0

"STR1"->Str1
"STR2"->Str2
"STR3"->Str3
""->Str4
"STR4"->Str5
"STR5"->Str6
0->A
1->B

sum(7,1,"605040C0C0","Spotify

Repeat K=45
    getKey->K
    If K=83:Then
        "PLAY->Str4
        Send(Str4
    End
    If K=84:Then
        "SKIP->Str4
        Send(Str4
    End
    If K=82:Then
        "REPLAY->Str4
        Send(Str4
    End
    Send(Str1
    sum(7,4,4,2,0,Str1
    Get(Str1
    Send(Str2
    sum(7,4,4,10,0,Str2
    Get(Str2
    Send(Str3
    sum(7,4,4,18,0,Str3
    Get(Str3
    sum(7,4,2,38,0,"["
    sum(7,4,90,38,0,"]"
    Send(A
    Get(A
    Send(B
    Get(B
    If B<=5:Then
        1->B
    End
    " "->Str7
    For(C,1,round(A*21/B))
        (Str7+"=")->Str7
    End
    sum(7,4,4,38,0,Str7
    sum(11
    sum(8,6
End
sum(10