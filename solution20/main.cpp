#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <algorithm>

#include <cassert>
#include <cctype>
#include <cstdio>
#include <set>
#include <queue>
#include <fstream>
#include <memory>

using int64 = int64_t;
using pss = std::pair<std::string, std::string>;
using pii = std::pair<int, int>;
using pii64 = std::pair<int64, int64>;
using vpii64 = std::vector<pii64>;
using vpii = std::vector<pii>;
using vs = std::vector<std::string>;
using vint = std::vector<int>;
using vvint = std::vector<vint>;
using vint64 = std::vector<int64>;
using vvint64 = std::vector<vint64>;

enum class Signal {
    Low,
    High
};

#define BROADCASTER "broadcaster"

struct Node : std::enable_shared_from_this<Node> {
    using Ptr = std::weak_ptr<Node>;
//    using SignalSending = std::pair<Ptr, Signal>;
    struct SignalSending {
        Ptr from;
        Ptr to;
        Signal signal;
    };
    using SignalSendings = std::vector<SignalSending>;
    std::string name;

    explicit Node(std::string name)
            : name(std::move(name)) {
    }

    virtual void addInputNode(Ptr node) {
        inputNodes.push_back(std::move(node));
    }

    virtual void addOutputNode(Ptr node) {
        outputNodes.push_back(std::move(node));
    }

    virtual SignalSendings receiveInput(Ptr source, Signal signal) = 0;

    virtual ~Node() = default;

    [[nodiscard]] SignalSendings resultForSingleSignal(const Signal &outgoingSignal) {
        SignalSendings result;
        for (const auto &outputNode: outputNodes) {
            result.push_back(SignalSending{shared_from_this(), outputNode, outgoingSignal});
        }
        return result;
    }

    std::vector<Ptr> outputNodes;
    std::vector<Ptr> inputNodes;
};

struct DummyNode : public Node {
    DummyNode(std::string name)
            : Node(std::move(name)) {}

    SignalSendings receiveInput(Ptr source, Signal signal) override {
        return {};
    }
};

struct BroadcasterNode : public Node {
    explicit BroadcasterNode()
            : Node(BROADCASTER) {
    }

    SignalSendings receiveInput(Ptr source, Signal signal) override {
        return resultForSingleSignal(signal);
    }
};

struct ConjunctionNode : public Node {
    explicit ConjunctionNode(std::string name)
            : Node(std::move(name)) {
    }

    void addInputNode(Ptr node) override {
        rememberedInputs[node.lock()->name] = Signal::Low;
        Node::addInputNode(node);
    }

    SignalSendings receiveInput(Ptr source, Signal incomingSignal) override {
        rememberedInputs.at(source.lock()->name) = incomingSignal;

        const bool allAreHigh = std::all_of(rememberedInputs.begin(), rememberedInputs.end(),
                                            [](const std::pair<std::string, Signal> &nameAndSignal) {
                                                return nameAndSignal.second == Signal::High;
                                            });

//        if (name == "ns") {
//            std::ostringstream os;
//            bool anyIsHigh = false;
//            for (auto [incomingName, signal] : rememberedInputs) {
//                os << incomingName << ":" << (signal == Signal::High ? "1" : "0") << ", ";
//                if (signal == Signal::High) {
//                    anyIsHigh = true;
//                }
//            }
//            if (anyIsHigh) {
//                std::cout << os.str() << std::endl;
//            }
//        }

        SignalSendings result;
        const auto outgoingSignal = allAreHigh ? Signal::Low : Signal::High;
        return resultForSingleSignal(outgoingSignal);
    }

    std::map<std::string, Signal> rememberedInputs;
};

struct FlipFlopNode : public Node {
    explicit FlipFlopNode(std::string name)
            : Node(std::move(name)) {
    }

    SignalSendings receiveInput(Ptr source, Signal incomingSignal) override {
        SignalSendings result;
        if (incomingSignal == Signal::High)
            return {};
        isOn = !isOn;

        const auto outgoingSignal = isOn ? Signal::High : Signal::Low;
        return resultForSingleSignal(outgoingSignal);
    }

    bool isOn = false;
};

int main() {
    std::ifstream fin{WORKDIR "input.txt"};
    assert(fin.is_open());
    std::cin.rdbuf(fin.rdbuf());

    std::vector<std::shared_ptr<Node>> nodes;
    std::map<std::string, Node::Ptr> nodesMap;

    std::map<std::string, std::vector<std::string>> outgoingConnections;

    std::string line;
    while (std::getline(std::cin, line)) {
        std::istringstream lineStream{line};
        char nodeType;
        lineStream >> nodeType;
        std::string nodeName;
        lineStream >> nodeName;

        std::shared_ptr<Node> node;
        if (nodeType == 'b') {
            assert(nodeName == "roadcaster");
            node = std::make_shared<BroadcasterNode>();
            nodeName = BROADCASTER;
        } else if (nodeType == '%') {
            node = std::make_shared<FlipFlopNode>(nodeName);
        } else if (nodeType == '&') {
            node = std::make_shared<ConjunctionNode>(nodeName);
        }
        nodes.push_back(node);
        nodesMap[node->name] = node;

        char c;
        while (lineStream >> c && c == ' ');
        assert(c == '-');
        lineStream >> c;
        assert(c == '>');
        while (lineStream >> c && c == ' ');

        std::vector<std::string> outgoingNodes;
        {
            std::string name;
            name += c;
            while (lineStream >> c) {
                if (c == ',') {
                    outgoingNodes.push_back(name);
                    name.clear();
                } else if (c == ' ') {
                    assert(name.empty());
                } else {
                    name += c;
                }
            }

            if (!name.empty()) {
                outgoingNodes.push_back(name);
            }

            while (lineStream >> name) {
                if (name.back() == ',')
                    name.pop_back();
                outgoingNodes.push_back(name);
            }
        }

        assert(outgoingConnections.count(nodeName) == 0);
        outgoingConnections[nodeName] = outgoingNodes;
    }

    for (const auto &[nodeName, outgoingNodes]: outgoingConnections) {
        assert(nodesMap.count(nodeName));
        auto nodePtr = nodesMap.at(nodeName).lock();
        for (const auto &outgoingName: outgoingNodes) {
            if (nodesMap.count(outgoingName) == 0) {
                auto newNode = std::make_shared<DummyNode>(outgoingName);
                nodesMap[outgoingName] = newNode;
                nodes.push_back(newNode);
            }
            auto outgoingNodePtr = nodesMap.at(outgoingName);
            nodePtr->addOutputNode(outgoingNodePtr);
            outgoingNodePtr.lock()->addInputNode(nodePtr);
        }
    }

//    for (auto [nodeName, nodePtr] : nodesMap) {
//        if (nodePtr.lock()->outputNodes.empty()) {
//            std::cout << "No output: " << nodeName << std::endl;
//        }
//
//        if (nodePtr.lock()->inputNodes.empty()) {
//            std::cout << "No input: " << nodeName << std::endl;
//        }
//    }

//    std::map<std::string, bool> used;

//    std::queue<std::string> q;
//    q.push("rx");
//    while (!q.empty()) {
//        auto nodeName = q.front();
//        q.pop();
//        auto nodePtr = nodesMap.at(nodeName).lock();
//        for (auto incomingPtr : nodePtr->inputNodes) {
//            auto incomingName = incomingPtr.lock()->name;
//            if (!used[incomingName]) {
//                used[incomingName] = true;
//                q.push(incomingName);
//            }
//        }
//    }

//    return 0;

    std::map<Signal, int64> signalCount;
    for (int64 i = 0; i < 1000; ++i) {

        // to, from, signal
        std::queue<Node::SignalSending> q;
        auto nodePtr = nodesMap.at(BROADCASTER);

        q.push(Node::SignalSending{{}, nodePtr, Signal::Low});

        bool found = false;
        while (!q.empty()) {
            Node::SignalSending signalSending = q.front();
            q.pop();

            auto fromPtr = signalSending.from;
            auto toPtr = signalSending.to;
            auto signal = signalSending.signal;
            signalCount[signal]++;

//            if (toPtr.lock()->name == "rx" && signal == Signal::Low) {
//                found = true;
//            }

            const auto signalSendings = toPtr.lock()->receiveInput(fromPtr, signal);
            for (auto ss: signalSendings) {
                q.push(std::move(ss));
            }
        }
//        if (found) {
//            std::cout << i + 1 << std::endl;
//            break;
//        }

//        if (i % 1'000 == 0) {
//            std::cout << i / 1'000 << std::endl;
//        }
    }

    std::cout << signalCount.at(Signal::Low) * signalCount.at(Signal::High) << std::endl;

//    std::cout << result << std::endl;

    return 0;
}