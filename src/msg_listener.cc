#include "msg_listener.h"
#include "message_ext.h"

void registerMessageListenerType(Php::Namespace &rocketMQNamespace){
    Php::Class<MessageListenerType> messageListenerTypeClass("MessageListenerType");
    messageListenerTypeClass.constant("LISTENER_DEFAULTLY", (int) rocketmq::messageListenerDefaultly);
    messageListenerTypeClass.constant("LISTENER_ORDERLY", (int) rocketmq::messageListenerOrderly);
    messageListenerTypeClass.constant("LISTENERCONCURRENTLY", (int) rocketmq::messageListenerConcurrently);

    rocketMQNamespace.add(messageListenerTypeClass);
}

rocketmq::ConsumeStatus commonConsumeMessage(const std::vector<rocketmq::MQMessageExt> &msgs, Php::Value callback){
    for (size_t i = 0; i < msgs.size(); ++i) {
        Php::Value msgExt(Php::Object(MESSAGE_EXT_CLASS_NAME, new MessageExt(msgs[i])));
        int ret = callback(msgExt);
        if (rocketmq::CONSUME_SUCCESS != ret){
            return rocketmq::RECONSUME_LATER;
        }
    }
    return rocketmq::CONSUME_SUCCESS;
}

rocketmq::ConsumeStatus MsgListenerConcurrently::consumeMessage(const std::vector<rocketmq::MQMessageExt> &msgs) {
    return commonConsumeMessage(msgs, this->callback);
}

rocketmq::ConsumeStatus MsgListenerOrderly::consumeMessage(const std::vector<rocketmq::MQMessageExt> &msgs) {
    return commonConsumeMessage(msgs, this->callback);
}

rocketmq::ConsumeStatus MsgListener::consumeMessage(const std::vector<rocketmq::MQMessageExt> &msgs) {
    return commonConsumeMessage(msgs, this->callback);
}
