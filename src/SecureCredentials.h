#pragma once

const char CA_CERT_PROG[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIEZTCCA02gAwIBAgIQQAF1BIMUpMghjISpDBbN3zANBgkqhkiG9w0BAQsFADA/
MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT
DkRTVCBSb290IENBIFgzMB4XDTIwMTAwNzE5MjE0MFoXDTIxMDkyOTE5MjE0MFow
MjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxCzAJBgNVBAMT
AlIzMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuwIVKMz2oJTTDxLs
jVWSw/iC8ZmmekKIp10mqrUrucVMsa+Oa/l1yKPXD0eUFFU1V4yeqKI5GfWCPEKp
Tm71O8Mu243AsFzzWTjn7c9p8FoLG77AlCQlh/o3cbMT5xys4Zvv2+Q7RVJFlqnB
U840yFLuta7tj95gcOKlVKu2bQ6XpUA0ayvTvGbrZjR8+muLj1cpmfgwF126cm/7
gcWt0oZYPRfH5wm78Sv3htzB2nFd1EbjzK0lwYi8YGd1ZrPxGPeiXOZT/zqItkel
/xMY6pgJdz+dU/nPAeX1pnAXFK9jpP+Zs5Od3FOnBv5IhR2haa4ldbsTzFID9e1R
oYvbFQIDAQABo4IBaDCCAWQwEgYDVR0TAQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8E
BAMCAYYwSwYIKwYBBQUHAQEEPzA9MDsGCCsGAQUFBzAChi9odHRwOi8vYXBwcy5p
ZGVudHJ1c3QuY29tL3Jvb3RzL2RzdHJvb3RjYXgzLnA3YzAfBgNVHSMEGDAWgBTE
p7Gkeyxx+tvhS5B1/8QVYIWJEDBUBgNVHSAETTBLMAgGBmeBDAECATA/BgsrBgEE
AYLfEwEBATAwMC4GCCsGAQUFBwIBFiJodHRwOi8vY3BzLnJvb3QteDEubGV0c2Vu
Y3J5cHQub3JnMDwGA1UdHwQ1MDMwMaAvoC2GK2h0dHA6Ly9jcmwuaWRlbnRydXN0
LmNvbS9EU1RST09UQ0FYM0NSTC5jcmwwHQYDVR0OBBYEFBQusxe3WFbLrlAJQOYf
r52LFMLGMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjANBgkqhkiG9w0B
AQsFAAOCAQEA2UzgyfWEiDcx27sT4rP8i2tiEmxYt0l+PAK3qB8oYevO4C5z70kH
ejWEHx2taPDY/laBL21/WKZuNTYQHHPD5b1tXgHXbnL7KqC401dk5VvCadTQsvd8
S8MXjohyc9z9/G2948kLjmE6Flh9dDYrVYA9x2O+hEPGOaEOa1eePynBgPayvUfL
qjBstzLhWVQLGAkXXmNs+5ZnPBxzDJOLxhF2JIbeQAcH5H0tZrUlo5ZYyOqA7s9p
O5b85o3AM/OJ+CktFBQtfvBhcJVd9wvlwPsk+uyOy2HI7mNxKKgsBTt375teA2Tw
UdHkhVNcsAKX1H7GNNLOEADksd86wuoXvg==
-----END CERTIFICATE-----
)EOF";

const char CLIENT_CERT_PROG[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFOTCCBCGgAwIBAgISBN5Jz/pctce9YM+yFa6ILvYiMA0GCSqGSIb3DQEBCwUA
MDIxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MQswCQYDVQQD
EwJSMzAeFw0yMTAyMjYwNjM1MzdaFw0yMTA1MjcwNjM1MzdaMCUxIzAhBgNVBAMT
Gm1xdHQuYWxtb25kaHlkcm9wb25pY3MuY29tMIIBIjANBgkqhkiG9w0BAQEFAAOC
AQ8AMIIBCgKCAQEA1OcUsOIzpJlkMRTUo/Vy+Ru3+fqbrt2JghNW6QGadxShBgSh
/Ii8UcJhJIa8B1GBkfVh8LlKbRbWGOnRICIYSGsr3iAanLQWms4211z36czbUwfk
3+JCSJDHKVcz1uNmadYPLL57z+39bnrIMKyxYoE7oty+XPc4+NxEDHZ3kU/JIn83
EhsxpFG7tjtcIeyeBjFsnWRRibjaEi+PNZgYcX3xB2KTHIv+mY2RWuX8OGZLOxDk
+V7RKU1UCwDvg+X2alh3ussN6xOf6IGFGgFeSqX8kbdvlaLMTCvujcFEVGURJcjy
Gf9ckgm+a/JwjbR061HFsncnnktvnHYjOnk+9wIDAQABo4ICVDCCAlAwDgYDVR0P
AQH/BAQDAgWgMB0GA1UdJQQWMBQGCCsGAQUFBwMBBggrBgEFBQcDAjAMBgNVHRMB
Af8EAjAAMB0GA1UdDgQWBBQTR9CnLy+j/PqYa3xOkrvgIN2hPDAfBgNVHSMEGDAW
gBQULrMXt1hWy65QCUDmH6+dixTCxjBVBggrBgEFBQcBAQRJMEcwIQYIKwYBBQUH
MAGGFWh0dHA6Ly9yMy5vLmxlbmNyLm9yZzAiBggrBgEFBQcwAoYWaHR0cDovL3Iz
LmkubGVuY3Iub3JnLzAlBgNVHREEHjAcghptcXR0LmFsbW9uZGh5ZHJvcG9uaWNz
LmNvbTBMBgNVHSAERTBDMAgGBmeBDAECATA3BgsrBgEEAYLfEwEBATAoMCYGCCsG
AQUFBwIBFhpodHRwOi8vY3BzLmxldHNlbmNyeXB0Lm9yZzCCAQMGCisGAQQB1nkC
BAIEgfQEgfEA7wB2AJQgvB6O1Y1siHMfgosiLA3R2k1ebE+UPWHbTi9YTaLCAAAB
d91DtKMAAAQDAEcwRQIhALK94q6Rc0xEkTighVVLr8huORy3F5eM3dqWp3NzRWzp
AiBZwPQDltNQ1WogoAIK7p4l+qoS3td0Rrll8+Z5pLoADQB1APZclC/RdzAiFFQY
CDCUVo7jTRMZM7/fDC8gC8xO8WTjAAABd91DtKMAAAQDAEYwRAIgLlwiEVx0QDNm
uXwaqaqSW5ROa3mtHqmDezv7WIKdkL8CIAmXgDSv/A2ncNg1YeGNzfXU1QGPPzwk
3bQN8Wc5RVxwMA0GCSqGSIb3DQEBCwUAA4IBAQAk901wfCfRbYz6DPkgSZfAno9G
Hab/XXj8v+vCnFNtl9etA6K/AooaYKchYkg9PM8TFBhr4vsVRTIpP4CuTYtmObFS
U6r71lFYwUCcFXdCvUrYygOPrM/bafdZzJBGFXFYWhaToa+JXLC4TL2pvVwRVetf
aN1kvttWp75sKKwlYSbCeUssEv79t+sEr+gM3+Qg6L0bjlQFjxWscjoIhZUeMzKf
XHfSkVTAw2goAw2i+tPMoQA8QW1FQ2SASprdiypZLjx/6sJ6sX8CXGrg6W+miAJM
lqgrRTLlb7nPi8fbsu4v6iHupgaK7wa3D2LKr8aq1zfM8opqpVSHJ0TeXUeX
-----END CERTIFICATE-----
)EOF";

// KEEP THIS VALUE PRIVATE AND SECURE!!!
const char CLIENT_KEY_PROG[] PROGMEM = R"KEY(
-----BEGIN PRIVATE KEY-----
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDU5xSw4jOkmWQx
FNSj9XL5G7f5+puu3YmCE1bpAZp3FKEGBKH8iLxRwmEkhrwHUYGR9WHwuUptFtYY
6dEgIhhIayveIBqctBaazjbXXPfpzNtTB+Tf4kJIkMcpVzPW42Zp1g8svnvP7f1u
esgwrLFigTui3L5c9zj43EQMdneRT8kifzcSGzGkUbu2O1wh7J4GMWydZFGJuNoS
L481mBhxffEHYpMci/6ZjZFa5fw4Zks7EOT5XtEpTVQLAO+D5fZqWHe6yw3rE5/o
gYUaAV5KpfyRt2+VosxMK+6NwURUZRElyPIZ/1ySCb5r8nCNtHTrUcWydyeeS2+c
diM6eT73AgMBAAECggEAcgaBUmsT9aghvRfG+zlOJpTt5/iMwQ7twrjQ52JwdC6c
Pzjj+PjSw4czt/hhZBoHJ0Eg/2h9sAd3C0jPNEKOdtrPV9wh5KOfJ7IiNR4ZuSJ6
pPGpZy8rCOphgIavwPtxsmVvIyux7B8uiKlB5q/XIUbNxeAsJGZUIRT3bSVVzIDQ
lNKcj2lHUnLplu3Fjm2mIFTv7jYhDQjqVDunAm0W/3NJ+4QLLa3GFgYsrEi/i64H
eTRE3my2aGdvfu2pViRKfy2EyH2UmI2Qz1+658PRy+7dPSzBgYpcFGOaC3PY2iNe
5+o94y4h5U/+JpTfaIuZ+9Js9/PrVRTCa7AItSmCIQKBgQDzJ2+1cqN7ld8wvElx
4SQlYw4vJzDS6HTuBaTbLjEfSFckfWT/A6vEQjhuwwv92GkNvqXpDpEGlXUNF9Kh
9a/60eAELWLEYFhTMKfZuNW4xwbd0JIEEdJqBQyoVdd4DMuJFgHStPiM1gf/lH9o
g30fAwg6DNa9L7D/IVEwO1oIXQKBgQDgJoGYTi0QjjZbaN5OhZwffzPPEwZAYRGS
n3nJuBP2pdpS845CxvpmBAocEcmg3q3ztgm7kcEB0xSFkZ2C8eo+NAZpTDdEczsh
GLQMPYAQaKGIc2mjTr9zh1YEB6zaCQU0TxOaaP59NK9c0/wawlbTJ5dnkiHOhR/x
OMGNHOvfYwKBgQDJrY4UO8+pc/e5XpK0Z/8pG23qYmE2riQ+QGQqf7eysLfGxlL8
ybiwghR4mkLZ+6O3CMhn8/MKOPDrOTU1rKiYK3fZPyPKqfbYmWS/S3XJWxsw5AwY
YAfcc++5t+mN4172h1BfykbV2bisnijHMTiYflVWgMukA2iVVLcYgEH+6QKBgBRN
aI+TBSHqv6Gn7frOv0VbECZBYt+R6zWMnEtZEPXWL+Azjz8dJrD2zLOeKNUZVkgs
xNTobsLri4BNoCasIrtfmvUOojLg/QoSYrx5FZBm6bqiOX9BMrYUHAZha5aUGpSS
VQQ3XlWY4AlYag1CbPrTKzOGWOmLxUjkCyPs6rBhAoGBAISDvf2FcKgob4Ps8QkO
LOdAhJtXeaUHs4RDrDGNQuilIiiIaOkLGdZA7RgkiQZ3r5a/ncPK0xGisxVXc+RL
WJOsAotOpKbrYhQ4vQRbTATJb86zojwxv7OOFad3DMAyHdw1myMMRUUjZOt2g8W4
gyi3dkmA1/vf6MxvTRurHLGE
-----END PRIVATE KEY-----
)KEY";
