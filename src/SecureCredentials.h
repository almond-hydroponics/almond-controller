#ifndef SECURECREDENTIALS_H
#define SECURECREDENTIALS_H


const char CA_CERT_PROG[] PROGMEM = R"EOF(
)EOF";

const char CLIENT_CERT_PROG[] PROGMEM = R"EOF(
)EOF";

// KEEP THIS VALUE PRIVATE AND SECURE!!!
const char CLIENT_KEY_PROG[] PROGMEM = R"KEY(
)KEY";

#endif //SECURECREDENTIALS_H
