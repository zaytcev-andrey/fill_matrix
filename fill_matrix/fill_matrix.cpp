// fill_matrix.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <vector>
#include <iostream>
#include <iterator>
#include <string>
#include <stack>
#include <sstream>
#include <memory>
#include <algorithm>
#include <iterator>
#include <set>
#include <tuple>
#include <assert.h>
#include <string.h>

class matrix
{
public:

    const static int def_value = 0;
    
    matrix( size_t row_count, size_t column_count )
        : row_count_( row_count )
        , column_count_( column_count )
        , data_( matrix_type( row_count, std::vector< int >( column_count, def_value ) ) )
    {
    }

    size_t get_row_count() const
    {
        return row_count_;
    }

    size_t get_column_count() const
    {
        return column_count_;
    }

    int get_cell( size_t row, size_t column ) const
    {
        return data_[ row ][ column ];
    }

    int& get_cell( size_t row, size_t column )
    {
        return data_[ row ][ column ];
    }

    void set_cell( size_t row, size_t column, int value )
    {
        data_[ row ][ column ] = value;
    }

    class iterator : public std::iterator< 
        std::random_access_iterator_tag
        , int 
        , int
        , const int*
        , int& >
    {
    public:
        explicit iterator( matrix& m, size_t idx, size_t row, size_t col ) 
            : m_( m )
            , idx_( idx )
            , row_( row )
            , col_( col )
        {
        }

        iterator& operator++() 
        {
            ++idx_;
            row_ = idx_ / m_.get_column_count();
            col_ = idx_ % m_.get_column_count();
            return *this; 
        }
        
        iterator operator++( int ) 
        { 
            iterator retval = *this;
            ++(*this); 
            return retval; 
        }

        bool operator<( iterator other ) const 
        { 
            return idx_ < other.idx_; 
        }
        bool operator==( iterator other ) const 
        { 
            return idx_ == other.idx_; 
        }
        bool operator!=( iterator other ) const { return !(*this == other); }
        reference operator*() const { return m_.get_cell( row_, col_ ); }

    private:
        matrix& m_;
        size_t idx_;
        size_t row_;
        size_t col_;
    };
    iterator begin() { return iterator( *this, 0, 0, 0 ); }
    iterator end() { return iterator( *this, row_count_ * column_count_, row_count_, column_count_ ); }

private:
    typedef std::vector< std::vector< int > > matrix_type;
    size_t row_count_;
    size_t column_count_;
    matrix_type data_;
};

class helix_iterator : public std::iterator<
    std::random_access_iterator_tag
    , int
    , int
    , const int*
    , int& >
{
public:
    explicit helix_iterator( matrix::iterator& iter )
        : iterator_( iter )
    {
    }

    helix_iterator& operator++()
    {
        ++iterator_;
        return *this;
    }

    helix_iterator operator++( int )
    {
        iterator_++;
        return *this;
    }

    bool operator<( const helix_iterator& other ) const
    {
        return iterator_ < other.iterator_;
    }
    bool operator==( const helix_iterator& other ) const
    {
        return iterator_ == other.iterator_;
    }

    bool operator!=( helix_iterator other ) const { return iterator_ != other.iterator_; }
    reference operator*() const { return *iterator_; }

private:
    matrix::iterator& iterator_;
};

std::ostream& operator<<( std::ostream& strm, const matrix& m )
{
    const size_t col_cnt = m.get_column_count();
    const size_t row_cnt = m.get_row_count();

    for( size_t row = 0; row < row_cnt; ++row )
    {
        for( size_t col = 0; col < col_cnt; ++col )
        {
            strm << m.get_cell( row, col ) << " ";
        }

        strm << std::endl;
    }

    return strm;
}

size_t get_cell_count( const matrix& m )
{
    return m.get_row_count() * m.get_column_count();
}

void fill_matrix( matrix& m, int init_value )
{
    size_t cell_count = get_cell_count( m );
    const size_t col_cnt = m.get_column_count();
    const size_t row_cnt = m.get_row_count();

    bool fill_row = true;
    bool increase = true;
    size_t row = 0;
    size_t col = 0;
    
    while ( cell_count-- > 0 )
    {
        m.set_cell( row, col, init_value++ );
        std::cout << m << std::endl;

        // get next        
        if ( fill_row )
        {
            const int next_col = col + ( increase ? 1 : -1 );
            if ( next_col < col_cnt && next_col >= 0
                && m.get_cell( row, next_col ) == matrix::def_value )
            {
                col = next_col;
            }
            else
            {
                fill_row = false;
                increase = ( col >= col_cnt / 2 );
                increase ? ++row : --row;
            }
        }
        else
        {
            const int next_row = row + ( increase ? 1 : -1 );
            if ( next_row < row_cnt && next_row >= 0
                && m.get_cell( next_row, col ) == matrix::def_value)
            {
                row = next_row;
            }
            else
            {
                fill_row = true;
                increase = ( row <= row_cnt / 2 );
                increase ? ++col : --col;
            }
        }
    }
}

void fill_matrix_sp( matrix& m, int init_value )
{
    const size_t col_count = m.get_column_count();
    const size_t row_count = m.get_row_count();

    int step = 0;
    int cell_count = col_count * row_count;
    while (cell_count > 0)
    {
        const int top_begin = step;
        const int top_end = col_count - step;
        const size_t top_row = step;
        for (int i = top_begin; i < top_end && cell_count; ++i, --cell_count)
        {
            m.set_cell( top_row, i, init_value++ );
        }

        const int right_begin = top_row + 1;
        const int right_end = row_count - 1 - step;
        const size_t right_col = top_end - 1;
        for (int j = right_begin; j < right_end && cell_count; ++j, --cell_count)
        {
            m.set_cell( j, right_col, init_value++ );
        }

        const int bottom_begin = top_end - 1;
        const int bottom_end = top_begin - 1;
        const size_t bottom_row = row_count - 1 - step;
        for (int i = bottom_begin; i > bottom_end && cell_count; --i, --cell_count)
        {
            m.set_cell( bottom_row, i, init_value++ );
        }

        const int left_begin = right_end - 1;
        const int left_end = right_begin - 1;
        const size_t left_col = top_begin;
        for (int j = left_begin; j > left_end && cell_count; --j, --cell_count)
        {
            m.set_cell( j, left_col, init_value++ );
        }

        ++step;
    }
}

void fill_matrix_iter( matrix& m, int value )
{
    std::fill( m.begin(), m.end(), value );
}

class list
{
public:
    list()
        : head_( 0 )
    {
    }

    ~list()
    {
        node* node_ptr = head_;
        while( !node_ptr )
        {
            node* next_node = head_->next;
            delete node_ptr;
            node_ptr = next_node;
        }
    }

    list& insert( int value )
    {
        node* value_node = new node;
        value_node->value = value;
        value_node->next = head_;
        head_ = value_node;

        return *this;
    }

    bool empty() const
    {
        return head_ == 0;
    }

    void print( std::ostream& strm ) const
    {
        node* node_ptr = head_;
        while ( node_ptr )
        {
            strm << node_ptr->value << " ";
            node_ptr = node_ptr->next;
        }
    }

    void reverse()
    {
        node* curr_node = head_;
        node* prev_node = 0;
        while ( curr_node )
        {
            node* next_node_ptr = curr_node->next;            
            curr_node->next = prev_node;

            if ( !next_node_ptr )
            {
                head_ = curr_node;
            }
            
            prev_node = curr_node;
            curr_node = next_node_ptr;            
        }          
    }

    void reverse_recursive()
    {
        reverse_recursive_impl( nullptr, head_ );
    }

private:
    struct node
    {
        node* next;
        int value;
    };

    node* head_;

private:
    void reverse_recursive_impl( node* prev, node* current )
    {               
        if ( current->next )
        {
            reverse_recursive_impl( current, current->next );
        }
        else
        {
            head_ = current;
        }        

        current->next = prev;        
    }
};

class leak_free_list
{
public:
	leak_free_list()
		: head_(nullptr)
	{
	}

    leak_free_list( std::initializer_list< int > in_list )
        : head_( nullptr )
    {
        for (const auto& val : in_list)
        {
            insert( val );
        }
    }

	~leak_free_list()
	{
	}

	leak_free_list& insert(int value)
	{
		std::unique_ptr< node > value_node = std::make_unique< node >();
		value_node->value = value;

		std::swap( head_, value_node->next );
		std::swap( value_node, head_ );
		
		return *this;
	}

	bool empty() const
	{
		return head_.get() == nullptr;
	}

	void print(std::ostream& strm) const
	{
		const node* node_ptr = head_.get();
		while ( node_ptr )
		{
			strm << node_ptr->value << " ";
			node_ptr = node_ptr->next.get();
		}
	}

	void reverse()
	{
        std::unique_ptr< node > curr_node = std::move( head_ );
        std::unique_ptr< node > prev_node( nullptr );

		while ( curr_node )
		{
            std::unique_ptr< node > next_node_ptr( std::move( curr_node->next ) );
			curr_node->next = std::move( prev_node );

			if (!next_node_ptr)
			{
				head_ = std::move( curr_node );
			}

			prev_node = std::move( curr_node );
			curr_node = std::move( next_node_ptr );
		}
	}

private:
	struct node
	{
		std::unique_ptr< node > next;
		int value;
        
        ~node()
        {
            std::cout << "~node() for " << value << std::endl;
        }

        node() = default;
        node( node&& ) = default;
	};

	std::unique_ptr< node > head_;

};

std::ostream& operator<<( std::ostream& strm, const list& ll )
{
    ll.print( strm );
    return strm;
}

std::ostream& operator<<( std::ostream& strm, const leak_free_list& ll )
{
    ll.print( strm );
    return strm;
}

std::string reverse_words( const std::string& text )
{
    std::stack< std::string > stack_str;

    std::stringstream strm( text );
    while ( !strm.eof() )
    {
        std::string str;
        strm >> str;        
        stack_str.push( str );
    }

    std::string res; 
    while ( !stack_str.empty() )
    {
        res += stack_str.top();
        stack_str.pop();
        if ( !stack_str.empty() )
        {
            res += " ";
        }        
    }

    return res;
}

void reverse_words( char* text )
{

}

template< class T >
std::string to_string( const T& value )
{
    std::stringstream sstrm;
    sstrm << value;

    return sstrm.str();
}

void str_compress( std::string& text )
{
    const size_t len = text.length();

    size_t ins_idx = 0;
    size_t prev_count = 0;
    char prev_sym = '0';

    for (size_t i = 0; i < len; ++i)
    {
        const char curr = text[ i ];

        if (curr == prev_sym)
        {
            prev_count++;
        }

        const bool end_of_text = ( i == len - 1 );
        const bool different_symb = ( curr != prev_sym );

        if (end_of_text || different_symb)
        {            
            if (prev_count)
            {
                const size_t all_repeat_count = prev_count + 1;
                std::string num = to_string( all_repeat_count );
                text.replace( ins_idx, num.length(), num );
                ins_idx += num.length();
                prev_count = 0;
            }
        }

        if (different_symb)
        {
            text[ ins_idx ] = curr;
            ++ins_idx;
            prev_sym = curr;
        }
    }

    text.resize( ins_idx );
}

std::vector< int > merge( const std::vector< int >& left
    , const std::vector< int >& right )
{
    std::vector< int > result;   

    const size_t left_len = left.size();
    const size_t right_len = right.size();

    result.reserve( left_len + right_len );

    size_t i = 0;
    size_t j = 0;

    while ( i < left_len || j < right_len )
    {
        if ( ( i < left_len && j < right_len && right[ j ] > left[ i ] ) || j >= right_len)
        {
            result.push_back( left[ i++ ] );
        }
        else /*if ( ( i < left_len && j < right_len && right[ j ] <= left[ i ] ) || i >= left_len)*/
        {
            result.push_back( right[ j++ ] );
        }        
    }

    assert( result.size() == left_len + right_len );

    return result;
}

void intersection_count(
    const std::vector< int >& a
    , const std::vector< int >& b
    , std::vector< int >& r )
{
    std::vector< int > f;
    std::vector< int > s;

    const size_t len = a.size();
    for (size_t idx = 0; idx < len; ++idx)
    {
        r[ idx ] = 0;

        f.push_back( a[ idx ] );
        s.push_back( b[ idx ] );
        std::sort( f.begin(), f.end() );
        std::sort( s.begin(), s.end() );

        const size_t f_len = f.size();
        const size_t s_len = s.size();

        size_t i = 0;
        size_t j = 0;
        while (i < f_len && j < s_len)
        {
            if (f[ i ] < s[ j ])
            {
                i++;
            }
            else if (f[ i ] == s[ j ])
            {
                i++; 
                j++;
                ++r[ idx ];
            }
            else
            {
                j++;
            }
        }
    }
}

void intersection_count_with_set(
    const std::vector< int >& a
    , const std::vector< int >& b
    , std::vector< int >& r )
{
    std::set< int > f;
    std::set< int > s;

    const size_t len = a.size();
    for (size_t idx = 0; idx < len; ++idx)
    {
        r[ idx ] = 0;

        f.insert( a[ idx ] );
        s.insert( b[ idx ] );

        std::set< int > intersect_set;
        std::set_intersection( f.begin(), f.end(), s.begin(), s.end(), std::inserter( intersect_set, intersect_set.end() ) );

        r[ idx ] = intersect_set.size();
    }
}

void increase_uniq( const std::vector< int >& array, size_t& idx )
{
    const int init_value = array[ idx ];

    while (++idx < array.size() && array[ idx ] == init_value);
}

void uniq_without_repeat(
    const std::vector< int >& left
    , const std::vector< int >& right
    , std::vector< int >& uniq )
{
    const size_t left_len = left.size();
    const size_t right_len = right.size();

    size_t i = 0;
    size_t j = 0;

    while (i < left_len || j < right_len)
    {
        if ( ( i < left_len && j < right_len && left[ i ] < right[ j ] ) || j >= right_len )
        {
            uniq.push_back( left[ i ] );
            increase_uniq( left, i );
        }
        else if ( i < left_len && j < right_len && left[ i ] == right[ j ] )
        {
            increase_uniq( left, i );
            increase_uniq( right, j );
        }
        else
        {
            uniq.push_back( right[ j ] );
            increase_uniq( right, j );
        }
    }
}

void remove_smile( std::string& text )
{
    const size_t len = text.size();
    size_t i = 0;
    size_t ins_idx = 0;    

    const std::string smile_base( ":-" );
    const size_t smile_base_len = smile_base.size();

    const std::string smile_f( ":-)" );    
    bool smile_f_found = false;
    const std::string smile_s( ":-(" );
    bool smile_s_found = false;
    const size_t smile_len = smile_f.size();

    char prev_parenthesis = '0';
    
    while (i < len)
    {
        if ( !smile_f_found && !smile_s_found )
        {
            smile_f_found = text.substr( i, smile_len ) == smile_f;
            smile_s_found = text.substr( i, smile_len ) == smile_s;
            i += ( smile_f_found || smile_s_found ) ? smile_len : 0;
        }
        
        const char curr = text[ i ];       

        if ( ( smile_f_found && curr == ')' ) || ( smile_s_found && curr == '(' ) )
        {
            i++;
        }
        else if ((smile_f_found && curr != ')') || (smile_s_found && curr != '('))
        {
            smile_f_found = false; 
            smile_s_found = false;

            continue;
        }

        if (!smile_f_found && !smile_s_found)
        {
            text[ ins_idx++ ] = curr;
            i++;
        }        
    }

    text.resize( ins_idx );
}

class tree
{
public:

    tree()
        : root_( nullptr )
        , size_()
    {
    }

    ~tree()
    {
        remove_post_order( root_ );
        assert( !size_ );
    }

    tree& insert( int val )
    {
        if ( !root_ )
        {
            root_ = new Node( val, size_++ );
        }
        else
        {
            insert_impl( val, root_ );
        }
        
        return *this;
    }

    size_t size() const
    {
        return size_;
    }

    void in_order_walk( std::vector< int >& values
        , std::vector< int >& id_to_values )
    {
        values.resize( size() );
        id_to_values.resize( size() );

        size_t idx = 0;
        in_order_walk_impl( root_, values, id_to_values, idx );
    }

    int find_parent( const std::vector< int >& values
        , std::vector< int >& id_to_values
        , int val_left, int val_right ) const
    {
        const Node* f = find_impl( val_left, root_ );
        const Node* s = find_impl( val_right, root_ );

        size_t idx_f = id_to_values[ f->id ];
        size_t idx_s = id_to_values[ s->id ];

        if (idx_f > idx_s)
        {
            std::swap( idx_f, idx_s );
            std::swap( f, s );
        }

        const Node* f_parent_res = nullptr;
        
        const Node* f_parent = f->parent;
        size_t idx_f_parent = idx_f + 1;

        while (f_parent != root_ && idx_f_parent < idx_s)
        {            
            if (f_parent->value != values[ idx_f_parent ])
            {
                ++idx_f_parent;
            }
            else if (f_parent->value == values[ idx_f_parent ])
            {
                f_parent_res = f_parent;
                f_parent = f_parent->parent;
                ++idx_f_parent;
            }           
        }

        return f_parent_res->value;
    }

private:

    struct Node
    {
        explicit Node( int val
            , size_t idx
            , Node* p = nullptr
            , Node* l = nullptr
            , Node* r = nullptr )
            : value( val )
            , id( idx )
            , left( l )
            , right( r )
            , parent( p )
        {}

        int value;
        size_t id;
        Node* left;
        Node* right;
        Node* parent;
    };

    Node* root_;
    size_t size_;

    void insert_impl( int val, Node* & node )
    {       
        if (node->value > val)
        {
            if (node->left)
            {
                insert_impl( val, node->left );
            }
            else
            {
                node->left = new Node( val, size_++, node );
            }
        }
        else
        {
            if (node->right)
            {
                insert_impl( val, node->right );
            }
            else
            {
                node->right = new Node( val, size_++, node );
            }
        }       
    }

    void remove_post_order( Node* node )
    {
        if (node)
        {
            remove_post_order( node->left );
            remove_post_order( node->right );

            delete node;
            --size_;
        }
    }

    void in_order_walk_impl( Node* node
        , std::vector< int >& values
        , std::vector< int >& id_to_values
        , size_t& idx )
    {
        if (node)
        {
            if (node->left)
            {
                in_order_walk_impl( node->left, values, id_to_values, idx );
                ++idx;
            }
            values[ idx ] = node->value;
            id_to_values[ node->id ] = idx;
            if (node->right)
            {
                in_order_walk_impl( node->right, values, id_to_values, ++idx );
            }
        }
    }

    const Node* find_impl( int val, const Node* node ) const
    {
        const Node* res = nullptr;

        if (node)
        {
            if (node->value > val)
            {
                res = find_impl( val, node->left );
            }
            else if (node->value == val)
            {
                res = node;
                return res;
            }
            else
            {
                res = find_impl( val, node->right );
            }
        }

        return res;
    }
};

template< class T >
void print( const T& values )
{
    for (const auto& value : values)
    {
        std::cout << value << " ";
    }

    std::cout << std::endl;
}

/*
есть строчка из нее нудно удалить все смайлики включа€ те где несколько одинаковых скобочек подр€д ":-):-((((("
. —ложность должна быть линейной, желательно in-place
*/

int _tmain(int argc, _TCHAR* argv[])
{
	matrix m( 3, 4 );
    fill_matrix( m, 1 );

    std::cout << m << std::endl;

    { // fill matrix sp
        matrix m( 3, 4 );
        fill_matrix_sp( m, 1 );

        std::cout << m << std::endl;

        matrix m2( 5, 7 );
        fill_matrix_sp( m2, 1 );

        std::cout << m2 << std::endl;
    }

    fill_matrix_iter( m, 5 );

    std::cout << m << std::endl;

    helix_iterator it( m.begin() );
    helix_iterator it_end( m.end() );
    std::fill( it, it_end, 7 );

    std::cout << m << std::endl;

    std::string str_to_compress( "aaabcc" );
    str_compress( str_to_compress );
    assert( str_to_compress == "a3bc2" );

    { // merge
        std::vector< int > left = { 1, 3, 5, 7, 9 };
        std::vector< int > right = { 2, 4, 6, 8 };

        std::vector< int > res = merge( left, right );
        const std::vector< int > expect = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
        assert( std::equal( res.begin(), res.end(), expect.begin() ) );
    }

    { // intersection
        std::vector< int > left = { 1, 2, 4, 4, 9 };
        std::vector< int > right = { 2, 4, 6, 8 };

        std::vector< int > intersection;
        intersection.reserve( left.size() + right.size() );
        std::set_intersection( left.begin(), left.end(), right.begin(), right.end(), std::back_inserter( intersection ) );
        print( intersection );
    }

    { // intersection count
        const std::vector< int > a = { 0, 1, 2, 3 };
        const std::vector< int > b = { 3, 1, 0, 2 };
        const std::vector< int > r = { 0, 1, 2, 4 };

        std::vector< int > result( a.size() );
        intersection_count( a, b, result );
        assert( std::equal( r.begin(), r.end(), result.begin() ) );
      
        std::vector< int > result2( a.size() );
        intersection_count_with_set( a, b, result2 );
        assert( std::equal( r.begin(), r.end(), result2.begin() ) );
    }

    { // uniq from arrays with repeatition
        std::vector< int > left = { 1, 2, 4, 4, 9 };
        std::vector< int > right = { 2, 4, 6, 8 };

        std::vector< int > uniq;
        uniq.reserve( left.size() + right.size() );
        uniq_without_repeat( left, right, uniq );
        
        const std::vector< int > expect = { 1, 6, 8, 9 };
        assert( std::equal( uniq.begin(), uniq.end(), expect.begin() ) );
    }

    { // remove smile
        std::string text( ":-):-(((((" );
        remove_smile( text );

        assert( text.empty() );

        text = ":-)))((:-((((())";
        remove_smile( text );

        const std::string expect( "(())" );
        assert( std::equal( text.begin(), text.end(), expect.begin() ) );
    }

    { // parent
        tree t;
        t.insert( 6 )
        .insert( 9 )
        .insert( 8 )
        .insert( 7 )
        .insert( 11 )
        .insert( 10 )
        .insert( 12 );

        std::vector< int > values;
        std::vector< int > id_to_values;
        t.in_order_walk( values, id_to_values );

        print( values );

        size_t parent_value = t.find_parent( values, id_to_values, 7, 12 );
        assert( parent_value == 9 );

        parent_value = t.find_parent( values, id_to_values, 10, 12 );
        assert( parent_value == 11 );        
    }

    list ll;
    ll.insert( 1 )
    .insert( 2 )
    .insert( 3 )
    .insert( 4 )
    .insert( 5 )
    .insert( 6 )
    .insert( 7 );

    std::cout << ll << std::endl;

    ll.reverse();
    std::cout << ll << std::endl;

    ll.reverse_recursive();
    std::cout << ll << std::endl;

    leak_free_list lf = { 1, 2, 3, 4, 5, 6, 7 };

    std::cout << lf << std::endl;

    lf.reverse();
    std::cout << lf << std::endl;

    const std::string text( "test string for words reveresing" );
    const std::string reverse_text = reverse_words( text );

    std::cout << text << std::endl;
    std::cout << reverse_text << std::endl;

    const std::vector< int > init_data = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
    const std::vector< int > result_data = { 1, 2, 4, 5, 7, 8, 10, 11 };
    const int decimation = 3;

    // remove with erase		
    {
        std::vector< int > data( init_data );
        int offset = 0;

        for (int idx = 0; idx < data.size(); ++idx)
        {
            if ((idx + 1 + offset) % decimation == 0)
            {
                data.erase( data.begin() + idx );
                ++offset;
            }
        }

        print( data );
        assert( std::equal( data.begin(), data.end(), result_data.begin() ) );
    }

    return 0;
}

