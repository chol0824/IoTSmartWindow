// Fill in  your WiFi networks SSID and password
#define SECRET_SSID "Soyoung"   // 와이파이 ID
#define SECRET_PASS "19990323"  // 와이파이 비번

// Fill in the hostname of your AWS IoT broker 
#define SECRET_BROKER "a32lfpoii34xf0-ats.iot.ap-northeast-2.amazonaws.com" // 엔드포인트

// Fill in the boards public certificate
const char SECRET_CERTIFICATE[] = R"( // 인증서
-----BEGIN CERTIFICATE-----
MIICijCCAXKgAwIBAgIVAPDK5UxaPSxXqxasR4zSOiEvZZ4SMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMDEyMDcwNTA0
MTRaFw00OTEyMzEyMzU5NTlaMBkxFzAVBgNVBAMTDklvVFNtYXJ0V2luZG93MFkw
EwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEVN/qJsKhomYRg201eWkCmLLrbFBO+i0h
/iXSe0NvvnuojmsqAmL6lZzYIpd1pqmAmgL71qypLmG3w0PIRK/v5aNgMF4wHwYD
VR0jBBgwFoAUs9qFbcqN8LjF7vP+WCv6MqyA2+AwHQYDVR0OBBYEFN3zNY6n5zMN
evIA8q07ufQfAzJmMAwGA1UdEwEB/wQCMAAwDgYDVR0PAQH/BAQDAgeAMA0GCSqG
SIb3DQEBCwUAA4IBAQCp2QSl4FBN3Cumla66J797FpsH56jWdfzEMYTHiRnydwL9
MP0QvTijG3T+nm4fmMLBH8b3S/d1L6Xw3aicUnRz5XQROI4/mbpTd2kWfoHiI80u
pqXfTnSZ7ixk5GCl08XTOXX3LGp1hDz/rZ/XEKtE3bX3PaIO3riwjIn1Gnxw581p
3iEIp/0n7PzfqZDVT0JZmuR0cBNXBlvtCj6HfaRkS37Y75oJ129nHXiOyyeokLBJ
JEnReuq4HGJjDOKGmSn9r95OQdlJptRbu0fz8y6NmqGX4cD3yACTSaN2QcfER3Mt
Kl8d6v0AvKCN0NrBvKP/ghlmK9kii/xz1EkEJ87h
-----END CERTIFICATE-----
)";
