//
// Created by chistopat on 26.06.2021.
//

#include "base_request.h"

RequestHolder BaseRequest::Create(BaseRequest::Type type) {
    switch (type) {
        case BaseRequest::Type::COMPUTE_INCOME:
            return make_unique<ComputeIncomeRequest>();
        case BaseRequest::Type::EARN:
            return make_unique<AddMoneyRequest<+1>>();
        case BaseRequest::Type::SPEND:
            return make_unique<AddMoneyRequest<-1>>();
        case BaseRequest::Type::PAY_TAX:
            return make_unique<PayTaxRequest>();
        default:
            return nullptr;
    }
}
