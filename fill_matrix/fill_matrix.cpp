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
#include <assert.h>

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

template< class T >
void print( const T& values )
{
    for (const auto& value : values)
    {
        std::cout << value << " ";
    }

    std::cout << std::endl;
}

int _tmain(int argc, _TCHAR* argv[])
{
	matrix m( 3, 4 );
    fill_matrix( m, 1 );

    std::cout << m << std::endl;

    fill_matrix_iter( m, 5 );

    std::cout << m << std::endl;

    helix_iterator it( m.begin() );
    helix_iterator it_end( m.end() );
    std::fill( it, it_end, 7 );

    std::cout << m << std::endl;

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

