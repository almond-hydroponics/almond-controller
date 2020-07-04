#ifndef SECURECREDENTIALS_H
#define SECURECREDENTIALS_H


const char CA_CERT_PROG[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/
MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT
DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow
SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT
GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC
AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF
q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8
SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0
Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA
a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj
/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T
AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG
CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv
bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k
c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw
VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC
ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz
MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu
Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF
AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo
uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/
wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu
X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG
PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6
KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==
-----END CERTIFICATE-----
)EOF";

const char CLIENT_CERT_PROG[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFfzCCBGegAwIBAgISBDK71lqVe1e4Tjc1c5S3xU/HMA0GCSqGSIb3DQEBCwUA
MEoxCzAJBgNVBAYTAlVTMRYwFAYDVQQKEw1MZXQncyBFbmNyeXB0MSMwIQYDVQQD
ExpMZXQncyBFbmNyeXB0IEF1dGhvcml0eSBYMzAeFw0yMDA1MTUwNjA1NDJaFw0y
MDA4MTMwNjA1NDJaMCAxHjAcBgNVBAMTFWFsbW9uZGh5ZHJvcG9uaWNzLmNvbTCC
ASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAKzt6qaMDoz/tqMtzi5Bg+/B
+kwFTYNIATOo6Rw4Xlm8Bd4pMxT+z0x47Iq+jh1puG4tIvlh0IWJ8ZX7hg49FEXt
ZwgLqz/KpUecLfdoPfbJnNMgekCg0FC4Xpobak40HLORwt6ECf3KJ2e1eIW0Rqh9
n0iOx27BSdc6cwqwQtTbndRJmZbzDD8NKPanM+PK50MyKuRYfRKI+/7KaOmM2vjb
a5eA2oI3szodIZfTOu5zL/2aPrqg++PbQfH5a2hUNxpEX8FC//Zr8VXQJh9dXQfM
lZjoumDMeR4Ssfnwx8loyxnOanff9b3fbJZv9CIaE12tPJpicX9vltjI83raAbEC
AwEAAaOCAocwggKDMA4GA1UdDwEB/wQEAwIFoDAdBgNVHSUEFjAUBggrBgEFBQcD
AQYIKwYBBQUHAwIwDAYDVR0TAQH/BAIwADAdBgNVHQ4EFgQU3SqoYz/5pJpnuuRL
gyHy0zBti9AwHwYDVR0jBBgwFoAUqEpqYwR93brm0Tm3pkVl7/Oo7KEwbwYIKwYB
BQUHAQEEYzBhMC4GCCsGAQUFBzABhiJodHRwOi8vb2NzcC5pbnQteDMubGV0c2Vu
Y3J5cHQub3JnMC8GCCsGAQUFBzAChiNodHRwOi8vY2VydC5pbnQteDMubGV0c2Vu
Y3J5cHQub3JnLzA7BgNVHREENDAyghVhbG1vbmRoeWRyb3Bvbmljcy5jb22CGXd3
dy5hbG1vbmRoeWRyb3Bvbmljcy5jb20wTAYDVR0gBEUwQzAIBgZngQwBAgEwNwYL
KwYBBAGC3xMBAQEwKDAmBggrBgEFBQcCARYaaHR0cDovL2Nwcy5sZXRzZW5jcnlw
dC5vcmcwggEGBgorBgEEAdZ5AgQCBIH3BIH0APIAdwCyHgXMi6LNiiBOh2b5K7mK
JSBna9r6cOeySVMt74uQXgAAAXIXJysZAAAEAwBIMEYCIQC3zHU4MIoSX94AzWlO
PJ1sBz2CwEV2YygzQJw3O/xs5wIhALa/uM/yGrz4crpTXrCS/uq6EZO8xpI0XaJg
1dP9Aw6WAHcAb1N2rDHwMRnYmQCkURX/dxUcEdkCwQApBo2yCJo32RMAAAFyFycr
8wAABAMASDBGAiEA//CV/vpuhJE9ttgsOg9dQWJq5pVeh5ZD9AwjfZIRZAsCIQDw
PyMEk/9+GsbBahRd3UMbPLh5+XJkJzf9lpkhGckWqTANBgkqhkiG9w0BAQsFAAOC
AQEAUyq7RUC+srwO3Ov3cgz+ub7ZfS3Tttt+PeRVZLvSPJREZdwBlc2ylrwBHdEF
AhVfAg94wadb5269vaekvEa9erJiinQjq2Gd6my6bsM7ZWhtaR3B/kcgU0/E1NbG
vt8QPbdnX+UNM4Yw4rSfNO/3dIU6LhQZfTmWWG6/lSSEhTHExSISWHWWVAYxPF0p
amFkV1i/VKa2VUVNYC2vZ0c2Qn/bT+fLEluSPjk/F0xQfZ0IVmGhY3p/iX0JzlWc
xtX0w8T725VwO+0YLrdiRxHJoERmJY7K9VujPPNNxJbfbq/IN1inM6DIItYoGwyT
D1qZG06qSUBuDMxbDlilUp5+SQ==
-----END CERTIFICATE-----

)EOF";

// KEEP THIS VALUE PRIVATE AND SECURE!!!
const char CLIENT_KEY_PROG[] PROGMEM = R"KEY(
----BEGIN PRIVATE KEY-----
MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQCs7eqmjA6M/7aj
Lc4uQYPvwfpMBU2DSAEzqOkcOF5ZvAXeKTMU/s9MeOyKvo4dabhuLSL5YdCFifGV
+4YOPRRF7WcIC6s/yqVHnC33aD32yZzTIHpAoNBQuF6aG2pONByzkcLehAn9yidn
tXiFtEaofZ9IjsduwUnXOnMKsELU253USZmW8ww/DSj2pzPjyudDMirkWH0SiPv+
ymjpjNr422uXgNqCN7M6HSGX0zrucy/9mj66oPvj20Hx+WtoVDcaRF/BQv/2a/FV
0CYfXV0HzJWY6LpgzHkeErH58MfJaMsZzmp33/W932yWb/QiGhNdrTyaYnF/b5bY
yPN62gGxAgMBAAECggEAJqy6+u593pjd/taiswOh/0ARu2jAMikrY6NWJyKQL192
rH3oV7y5HfA5bro5TGi1f6aGmOGAYi08o39yNsMAQrdagJkjfovStRY/zYI5wXTL
pwgvbz7H+BWTysFw3/aVqK9jtqIvHdxVPcN/Jd/woYQe2HpeLe36lrESw4QyHip1
n+3vv60zpYyfRdy3kc3P+O+lfrLf4g9sRWq5Ssc341KBDJyF/oAJZQNV90ASaXge
38GeKGAg2sdBzC58iutjLVvgD1MAvpmuGTF22l7gddzkX19Wc9uyujyRtOBd+t1U
2dK5LXzR9e88RWrXgW5rhx62TbJEqey5UW2383wf8QKBgQDSvPlmE0PK2ooKTA0p
FyBSpTXYbhhzCDpp6AOjZA56QPaJdHLLff8zk7oFWAwmfXWvixOTeDBn6iiB/7qJ
sA+lio71cxMNTvpJyNDUmIHQuACISY3p7LR/D+fIzqgn0Ahxtz5LJN71XoCsrFa4
wIiDMowluJ6YP4TnucbupKOY5QKBgQDSEhk9vSGeFt7xJDNaCKxLzjdQnzgU/hw6
SjbNzx8kJ3FkK3IyzdSzDTtklVNj/WP8kDWypdWHHzsY5KtnNK4G5xueivxMrlpl
HLK/GCClknnwoEexJqEKSE6T1vBWJBxBBGS4HRYTPXuHtvkzV4tdCTBCjq+9KS/G
ilqHvca03QKBgQDKmKx3qpupAFJnNdgNIdOMQJI0gRt2M3nc6RXeVcnpVFzvpoja
A0Z+Mk5K5uu/cSvb2Ku4QXrf2+iXmd+ekSlpr3xikf/F8qCCoNHLINbkbpDsdTI+
qraX1RWzIqHcrI4BrPXxTgAfCnxQBNrWgRp8bvL3LLcjjSVblWmfcZ5p1QKBgDG3
Is4fspMNapm7LYQDRXNzTFgCNm2RbfDFm469k+skgotoVNB9MfdmDEZTqLajQ0SE
hJDQ+mXOGFhd1crJBt6PV1QAJJGpOa9wSUU8Wo2UytMNu6nLMJWUYzRNy/c2+JDS
j1ubVpwbEt8diICEGXFdOVSnHCQyWSItoBs76IjpAoGBAMNn9VGdcwH08LAAHr59
jXe4OnX59xmx1hJn7nF6tfU+DYzmKX3z0yTv9zKdT/ffs55Rt46bzTOYThE3VOO9
qTb8L80dNuz30x+KeLfxtaQrljiewQBnn6J9ccBzliSmSMJR67KnCIL7axyiR8p7
lvnHHnYW5aBg+pnVtX+mBdBE
-----END PRIVATE KEY-----
)KEY";

#endif //SECURECREDENTIALS_H
