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

template<typename T=int64> T gcd(T a, T b) {
    return b == 0? a : gcd(b, a % b);
}

template<typename T=int64> T lcm(T a, T b) {
    return a / gcd(a, b) * b;
}

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
    std::ifstream fin{WORKDIR aoc_utils::INPUT_FILE};
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

    std::set<std::string> childrenNames{"dc", "rv", "vp", "cq"};
    std::map<std::string, vint64> childrenOccurences;
    for (const auto& childName : childrenNames) {
        childrenOccurences[childName];
    }

    for (int64 i = 0; i >= 0; ++i) {

        // to, from, signal
        std::queue<Node::SignalSending> q;
        auto nodePtr = nodesMap.at(BROADCASTER);

        q.push(Node::SignalSending{{}, nodePtr, Signal::Low});

        while (!q.empty()) {
            Node::SignalSending signalSending = q.front();
            q.pop();

            auto fromPtr = signalSending.from;
            auto toPtr = signalSending.to;
            auto signal = signalSending.signal;

            const auto signalSendings = toPtr.lock()->receiveInput(fromPtr, signal);
            if (!signalSendings.empty()) {
                auto ss = signalSendings.front();
                auto name = ss.from.lock()->name;
                if (ss.signal == Signal::High && childrenNames.count(name)) {
                    childrenOccurences[name].push_back(i);
                }
            }

            for (auto ss: signalSendings) {
                q.push(std::move(ss));
            }
        }

        if (std::all_of(childrenOccurences.begin(), childrenOccurences.end(), [](const std::pair<std::string, vint64>& sv) {
            return sv.second.size() > 1;
        })) {
            int64 result = 1;

            for (auto [childName, occurences] : childrenOccurences) {
                assert(childrenOccurences.size() >= 2);
                result = lcm(result, occurences[1] - occurences[0]);
            }

            std::cout << result << std::endl;
            break;
        }
    }

    return 0;
}