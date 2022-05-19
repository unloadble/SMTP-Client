
#ifndef SMTP_CLIENT_SMTP_CONSTANTS_H
#define SMTP_CLIENT_SMTP_CONSTANTS_H

namespace smtp::constants::status_codes {
    const char SERVICE_READY[] {"220"};
    const char CLOSED[] {"221"};
    const char ACCEPT_AUTH[] {"235"};
    const char ACTION_COMPLETE[] {"250"};
    const char AUTH[] {"334"};
    const char START_MAIL_INPUT[] {"354"};
}

#endif //SMTP_CLIENT_SMTP_CONSTANTS_H
