//
// Created by inejka on 28.11.2020.
//

#ifndef SEM3_LAB4_PPVIS_GRAPH_H
#define SEM3_LAB4_PPVIS_GRAPH_H

#include <iterator>

template<typename T>
struct Traits {
    typedef const T &arg_type;
    typedef T &reference;
    typedef const T &const_reference;
};

template<typename node_type, typename traits=Traits<node_type>>
class Graph {
private:
    typedef typename traits::arg_type arg_type;
    typedef typename traits::reference reference;
    typedef typename traits::const_reference const_reference;
    struct Head_node;

    struct Arc_node {
        Arc_node(Head_node *&id) : id(id) {}

        Head_node *id = nullptr;
        Arc_node *next = nullptr;
    };

    struct Head_node {
        Head_node(arg_type to_add) : key(to_add) {}

        node_type key;
        int count_prev = 0, count_next = 0;
        Head_node *next = nullptr;
        Arc_node *trail = nullptr, *prev = nullptr;

        bool operator!=(const Head_node &rhs) {
            return !(this == &rhs);
        }
    };

    Head_node *find_node(arg_type to_find) const {
        auto tmp = first;
        while (tmp && !(tmp->key == to_find))
            tmp = tmp->next;
        return tmp;
    }

    Arc_node *find_last_arc_node(Arc_node *to_find) {
        while (to_find->next)
            to_find = to_find->next;
        return to_find;
    }

    void find_and_del(Arc_node *to_delete, const Head_node *to_find) {
        Arc_node *delete_ptr = to_delete;
        while (!(delete_ptr->next->id == to_find))
            delete_ptr = delete_ptr->next;
        auto tmp = delete_ptr->next;
        delete_ptr->next = delete_ptr->next->next;
        delete tmp;
    }

    void clear_edges(Head_node *&to_delete) {
        if (to_delete->trail)del_rek_prev(to_delete->trail, to_delete->key);
        if (to_delete->prev)del_rek_next(to_delete->prev, to_delete->key);
    }

    void del_rek_prev(Arc_node *&to_delete, arg_type from) {
        if (to_delete->next)
            del_rek_prev(to_delete->next, from);
        delete_edge(from, to_delete->id->key);
    }

    void del_rek_next(Arc_node *&to_delete, arg_type to) {
        if (to_delete->next)
            del_rek_next(to_delete->next, to);
        delete_edge(to_delete->id->key, to);
    }

    Head_node *first, *last;
public:
    Graph() {
        first = last = nullptr;
    }

    void add_node(arg_type to_add) {
        if (is_in(to_add))return;
        if (first == last && first == nullptr) {
            last = new Head_node(to_add);
            first = last;
        } else if (first == last) {
            last = new Head_node(to_add);
            first->next = last;
        } else {
            last->next = new Head_node(to_add);;
            last = last->next;
        }
    }

    bool is_in(arg_type to_compare) const {
        auto tmp = first;
        while (tmp && !(tmp->key == to_compare))
            tmp = tmp->next;
        if(tmp)return true; else return false;
    }

    bool edge_is_in(arg_type from, arg_type to) const {
        auto from_node = find_node(from), to_node = find_node(to);
        if (!(from_node && to_node))return false;
        if (from_node->count_next == 0 || to_node->count_prev == 0) return false;
        Arc_node *tmp = from_node->trail;
        while (tmp) {
            if (tmp->id->key == to) return true;
            tmp = tmp->next;
        }
        return false;
    }

    void add_edge(arg_type from, arg_type to) {
        if (edge_is_in(from, to))return;
        auto from_ptr = find_node(from), to_ptr = find_node(to);
        if (!(from_ptr && to_ptr))return;
        if (from_ptr->count_next == 0)
            from_ptr->trail = new Arc_node(to_ptr);
        else find_last_arc_node(from_ptr->trail)->next = new Arc_node(to_ptr);
        if (to_ptr->count_prev == 0)
            to_ptr->prev = new Arc_node(from_ptr);
        else find_last_arc_node(to_ptr->prev)->next = new Arc_node(from_ptr);
        from_ptr->count_next++;
        to_ptr->count_prev++;
    }

    void delete_edge(arg_type from, arg_type to) {
        if (!edge_is_in(from, to))return;
        auto from_ptr = find_node(from), to_ptr = find_node(to);
        if (from_ptr->count_next == 1) {
            delete from_ptr->trail;
            from_ptr->trail = nullptr;
        } else if (from_ptr->trail->id == to_ptr) {
            auto tmp = from_ptr->trail;
            from_ptr->trail = tmp->next;
            delete tmp;
        } else find_and_del(from_ptr->trail, to_ptr);
        if (to_ptr->count_prev == 1) {
            delete to_ptr->prev;
            to_ptr->prev = nullptr;
        } else if (to_ptr->prev->id == from_ptr) {
            auto tmp = to_ptr->prev;
            to_ptr->prev = tmp->next;
            delete tmp;
        } else find_and_del(to_ptr->prev, from_ptr);
        from_ptr->count_next--;
        to_ptr->count_prev--;
    }

    void delete_node(node_type to_delete) {
        if (!is_in(to_delete))return;
        Head_node *tmp = nullptr;
        if (first == last) {
            tmp = first;
            first = last = nullptr;
        } else if (first->key == to_delete) {
            tmp = first;
            first = tmp->next;
        } else {
            Head_node *tmp1 = first;
            while (!(tmp1->next->key == to_delete))
                tmp1 = tmp1->next;
            tmp = tmp1->next;
            if (tmp == last) {
                last = tmp1;
                tmp1->next = nullptr;
            } else tmp1->next = tmp->next;
        }
        clear_edges(tmp);
        tmp->next = nullptr;
        delete tmp;
    }

    void clear() {
        while (first)
            delete_node(first->key);
    }

    long long get_node_count() const {
        long long to_return = 0;
        auto tmp = first;
        while (tmp) {
            to_return++;
            tmp = tmp->next;
        }
        return to_return;
    }

    long long get_edge_count() const {
        long long to_return = 0;
        auto tmp = first;
        while (tmp) {
            to_return += tmp->count_next;
            tmp = tmp->next;
        }
        return to_return;
    }

    long long get_node_degree(arg_type node) {
        auto tmp = first;
        while (tmp && !(tmp->key == node))
            tmp = tmp->next;
        if (tmp)return (tmp->count_next + tmp->count_prev); else return -1;
    }

    Graph(const Graph<node_type> &to_copy) {
        first = last = nullptr;
        auto tmp = to_copy.first;
        while (tmp) {
            add_node(tmp->key);
            tmp = tmp->next;
        }
        tmp = to_copy.first;
        while (tmp) {
            auto tmp1 = tmp->trail;
            while (tmp1) {
                add_edge(tmp->key, tmp1->id->key);
                tmp1 = tmp1->next;
            }
            tmp = tmp->next;
        }
    }


    Graph<node_type> &operator=(const Graph<node_type> &right) {
        if (this == &right) {
            return *this;
        }
        auto tmp = right.first;
        while (tmp) {
            add_node(tmp->key);
            tmp = tmp->next;
        }
        tmp = right.first;
        while (tmp) {
            auto tmp1 = tmp->trail;
            while (tmp1) {
                add_edge(tmp->key, tmp1->id->key);
                tmp1 = tmp1->next;
            }
            tmp = tmp->next;
        }
        return *this;
    }

    ~Graph() {
        clear();
    }

    class Edge_iterator_const : public std::iterator<std::bidirectional_iterator_tag, node_type> {
    protected:
        Head_node *current_node, *&first, *&last;
        Graph<node_type> &graph;
        Arc_node *current;
    public:
        Edge_iterator_const(Head_node *current_node, Head_node *&first, Head_node *&last, Graph<node_type> &graph,
                            Arc_node *current) : current_node(
                current_node), first(first), last(last), graph(graph), current(current) {}


        Edge_iterator_const &operator++(int) {
            if (current && current->next)
                current = current->next;
            else {
                if (current_node) {
                    current_node = current_node->next;
                    while (current_node && !current_node->trail)
                        current_node = current_node->next;
                }
                if (current_node)current = current_node->trail;
                else
                    current = nullptr;
            }
            return *this;
        }

        Edge_iterator_const &operator++() {
            if (current && current->next)
                current = current->next;
            else {
                if (current_node) {
                    current_node = current_node->next;
                    while (current_node && !current_node->trail)
                        current_node = current_node->next;
                }
                if (current_node)current = current_node->trail;
                else
                    current = nullptr;
            }
            return *this;
        }

        std::pair<node_type, node_type> operator*() {
            return std::pair<node_type, node_type>(current_node->key, current->id->key);
        }

        bool empty() {
            if (current)return false; else return true;
        }

        bool operator!=(const Edge_iterator_const &rhs) const {
            return !(current == rhs.current);
        }

        bool operator==(const Edge_iterator_const &rhs) const {
            return (current == rhs.current);
        }

        Edge_iterator_const &operator--() {
            if (graph.begin_edge() == *this)return *this;
            if (current_node && current_node->trail != current) {
                auto tmp = current_node->trail;
                while (tmp->next != current)tmp = tmp->next;
                current = tmp;
            } else {
                Head_node *tmp1 = first, *tmp2 = first;
                do {
                    tmp2 = tmp1;
                    tmp1 = tmp1->next;
                    while (tmp1 && !tmp1->trail)tmp1 = tmp1->next;
                } while (tmp1 != current_node);
                current_node = tmp2;
                current = current_node->trail;
                while (current->next)current = current->next;
            }
            return *this;
        }

        Edge_iterator_const &operator--(int) {
            if (graph.begin_edge() == *this)return *this;
            if (current_node && current_node->trail != current) {
                auto tmp = current_node->trail;
                while (tmp->next != current)tmp = tmp->next;
                current = tmp;
            } else {
                Head_node *tmp1 = first, *tmp2 = first;
                do {
                    tmp2 = tmp1;
                    tmp1 = tmp1->next;
                    while (tmp1 && !tmp1->trail)tmp1 = tmp1->next;
                } while (tmp1 != current_node);
                current_node = tmp2;
                current = current_node->trail;
                while (current->next)current = current->next;
            }
            return *this;
        }
    };

    Edge_iterator_const begin_edge_const() {
        auto tmp = first;
        while (tmp && !tmp->trail)tmp = tmp->next;
        if (tmp)
            return Edge_iterator_const(tmp, first, last, *this, tmp->trail);
        else return Edge_iterator_const(nullptr, first, last, *this, nullptr);

    }

    Edge_iterator_const end_edge_const() {
        return Edge_iterator_const(nullptr, first, last, *this, nullptr);
    }

    class Edge_iterator : public Edge_iterator_const {
    public:
        Edge_iterator(Head_node *current_node, Head_node *&first, Head_node *&last, Graph<node_type> &graph,
                      Arc_node *current) : Edge_iterator_const(current_node, first, last, graph,
                                                               current) {}

        void erase() {
            if (this->current)
                this->graph.delete_edge(this->current_node->key, this->current->id->key);
            this->current = nullptr;
        }

        std::pair<node_type &, node_type &> operator*() {
            return std::pair<node_type &, node_type &>(this->current_node->key, this->current->id->key);
        }
    };

    Edge_iterator begin_edge() {
        auto tmp = first;
        while (tmp && !tmp->trail)tmp = tmp->next;
        if (tmp)
            return Edge_iterator(tmp, first, last, *this, tmp->trail);
        else return Edge_iterator(nullptr, first, last, *this, nullptr);

    }

    Edge_iterator end_edge() {
        return Edge_iterator(nullptr, first, last, *this, nullptr);
    }

    class Edge_iterator_reversed : public Edge_iterator {
    public:
        Edge_iterator_reversed(Head_node *current_node, Head_node *&first, Head_node *&last, Graph<node_type> &graph,
                               Arc_node *current) : Edge_iterator(current_node, first, last, graph,
                                                                  current) {}

        Edge_iterator_reversed &operator++(int) {
            Edge_iterator_const::operator--();
            return *this;
        }

        Edge_iterator_reversed &operator++() {
            Edge_iterator_const::operator--();
            return *this;
        }

        Edge_iterator_reversed &operator--(int) {
            Edge_iterator_const::operator++();
            return *this;
        }

        Edge_iterator_reversed &operator--() {
            Edge_iterator_const::operator++();
            return *this;
        }
    };

    Edge_iterator_reversed begin_edge_reversed() {
        auto tmp = first;
        while (tmp && !tmp->trail)tmp = tmp->next;
        if (tmp)
            return Edge_iterator_reversed(tmp, first, last, *this, tmp->trail);
        else return Edge_iterator_reversed(nullptr, first, last, *this, nullptr);

    }

    Edge_iterator_reversed end_edge_reversed() {
        return Edge_iterator_reversed(nullptr, first, last, *this, nullptr);
    }


    class Node_iterator : public std::iterator<std::bidirectional_iterator_tag, node_type> {
    protected:
        Head_node *current, *&first, *&last;
        Graph<node_type> &graph;
    public:

        Node_iterator(Head_node *current, Head_node *&first, Head_node *&last, Graph<node_type> &graph) : current(
                current), first(first), last(last), graph(graph) {}

        Node_iterator &operator++(int) {
            if (current)
                current = current->next;
            return *this;
        }

        Node_iterator &operator++() {
            if (current)
                current = current->next;
            return *this;
        }

        Node_iterator &operator--(int a) {
            if (current == first)return *this;
            auto tmp = first;
            while (tmp->next != current) tmp = tmp->next;
            current = tmp;
            return *this;
        }

        Node_iterator &operator--() {
            if (current == first)return *this;
            auto tmp = first;
            while (tmp->next != current) tmp = tmp->next;
            current = tmp;
            return *this;
        }

        node_type &operator*() {
            return current->key;
        }

        bool operator!=(const Node_iterator &rhs) const {
            return !(current == rhs.current);
        }

        bool operator==(const Node_iterator &rhs) const {
            return (current == rhs.current);
        }

        void erase() {
            if (current)
                graph.delete_node(current->key);
            current = last->next;
        }

    };

    Node_iterator begin() {
        return Node_iterator(first, first, last, *this);
    }

    Node_iterator end() {
        return Node_iterator(nullptr, first, last, *this);
    }

    class Node_iterator_const : public Node_iterator {
        void erase() {}

    public:
        Node_iterator_const(Head_node *current, Head_node *&first, Head_node *&last, Graph<node_type> &graph)
                : Node_iterator(current, first, last, graph) {}

        node_type operator*() {
            return this->current->key;
        }
    };

    Node_iterator_const begin_const() {
        return Node_iterator_const(first, first, last, *this);
    }

    Node_iterator_const end_const() {
        return Node_iterator_const(nullptr, first, last, *this);
    }

    class Node_iterator_reversed : public Node_iterator {
    public:
        Node_iterator_reversed(Head_node *current, Head_node *&first, Head_node *&last, Graph<node_type> &graph)
                : Node_iterator(current, first, last, graph) {}

        Node_iterator_reversed &operator++(int) {
            Node_iterator::operator--();
            return *this;
        }

        Node_iterator_reversed &operator++() {
            Node_iterator::operator--();
            return *this;
        }

        Node_iterator_reversed &operator--(int) {
            Node_iterator::operator++();
            return *this;
        }

        Node_iterator_reversed &operator--() {
            Node_iterator::operator++();
            return *this;
        }
    };

    Node_iterator_reversed begin_reversed() {
        return Node_iterator_reversed(first, first, last, *this);
    }

    Node_iterator_reversed end_reversed() {
        return Node_iterator_reversed(nullptr, first, last, *this);
    }
};


#endif //SEM3_LAB4_PPVIS_GRAPH_H
