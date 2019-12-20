/*
 * Response.h
 *
 *  Created on: May 12, 2012
 *      Author: mnunberg
 */

#ifndef RESPONSE_H_
#define RESPONSE_H_

#ifndef SDKD_INTERNAL_H_
#error "Include sdkd_internal.h first"
#endif

namespace CBSdkd {

using std::string;

class Response : public Message {
public:

    Response();

    // Copy constructor gets called just fine here..
    Response(const Request*, Error const& = Error());
    Response(const Request&, Error const& = Error());
    // But not here?

    const string encode() const;

    void setResponseData(Json::Value& rdata);

private:
    Json::Value response_data;
};

} /* namespace CBSdkd */
#endif /* RESPONSE_H_ */
