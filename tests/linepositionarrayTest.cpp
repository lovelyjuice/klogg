#include <catch.hpp>

#include "config.h"

#include "log.h"

#include "data/linepositionarray.h"

#include <random>
#include <vector>
#include <algorithm>

SCENARIO( "LinePositionArray with small number of lines", "[linepositionarray]") {

    std::vector<LineOffset> offsets = {
        4_offset,
        8_offset,
        10_offset,
        345_offset, // A longer (>128) line
        20000_offset, // An even longer (>16384) line
        20020_offset
    };

    GIVEN( "LinePositionArray with small number of lines") {

        LinePositionArray line_array;

        for ( const auto& offset: offsets ) {
            line_array.append( offset );
        }

        REQUIRE( line_array.size() == 6_lcount );

        WHEN( "Access items in linear order" ) {
            THEN( "Corrent offsets returned") {
                for ( auto i = 0u; i<offsets.size(); ++i ) {
                    REQUIRE( line_array[i] == offsets[i] );
                }
            }
        }

        WHEN( "Access items in random order" ) {

            std::random_device rd;
            std::mt19937 g(rd());

            auto index = std::vector<int>( offsets.size() );
            std::generate(index.begin(), index.end(), [n = 0] () mutable { return n++; });

            THEN( "Corrent offsets returned") {
                for ( auto i : index ) {
                    REQUIRE( line_array[i] == offsets[i] );
                }
            }
        }

        WHEN( "Adding fake lf" ) {
            line_array.setFakeFinalLF();

            THEN( "Last offset is returned" ) {
                REQUIRE( line_array[5] == offsets[5] );
            }
        }

        WHEN( "Adding line after fake lf") {
            line_array.setFakeFinalLF();
            line_array.append( 20030_offset );

            THEN( "New last offset is returned" ) {
                REQUIRE( line_array[6] == 20030_offset );
            }
        }

        WHEN( "Appending other array ") {

            GIVEN( "FastLinePositionArray" ) {
                FastLinePositionArray other_array;
                other_array.append( 150000_offset );
                other_array.append( 150023_offset );

                WHEN( "Appending other array without fake lf" ) {
                    line_array.append_list( other_array );

                    THEN( "All lines are kept" ) {
                        for ( auto i = 0u; i<offsets.size(); ++i ) {
                            REQUIRE( line_array[i] == offsets[i] );
                        }
                        REQUIRE( line_array[6] == other_array[0] );
                        REQUIRE( line_array[7] == other_array[1] );
                    }
                }

                WHEN( "Appending after fake lf" ) {
                    line_array.setFakeFinalLF();
                    line_array.append_list( other_array );

                    THEN( "Last line is popped back" ) {
                        REQUIRE( line_array.size() == 7_lcount );
                        for ( auto i = 0u; i<offsets.size()-1; ++i ) {
                            REQUIRE( line_array[i] == offsets[i] );
                        }
                        REQUIRE( line_array[5] == other_array[0] );
                        REQUIRE( line_array[6] == other_array[1] );
                    }
                }
            }
        }
    }
}

SCENARIO( "LinePositionArray with full block of lines", "[linepositionarray]") {

    GIVEN( "LinePositionArray with block of lines") {

        LinePositionArray line_array;

        // Add 255 lines (of various sizes)
        const int lines = 255;
        for ( int i = 0; i < lines; ++i )
            line_array.append( LineOffset( i * 4 ) );
        // Line no 256
        line_array.append( LineOffset( 255 * 4 ) );

        WHEN( "Adding line after block") {
            // Add line no 257
            line_array.append( LineOffset( 255 * 4 + 10 ) );

            THEN( "Correct offset is returned" ) {
                REQUIRE( line_array[256] == LineOffset( 255 * 4 + 10 ) );
            }
        }

        WHEN( "Adding lines after fake lf") {
            THEN( "Correct offset is returned" )
            for ( uint64_t i = 0; i < 1000; ++i ) {
                uint64_t pos = ( 257LL * 4 ) + i*35LL;
                line_array.append( LineOffset( pos ) );
                line_array.setFakeFinalLF();
                REQUIRE( line_array[256 + i] == LineOffset( pos ) );
                line_array.append( LineOffset( pos + 21LL ) );
                REQUIRE( line_array[256 + i] == LineOffset( pos + 21LL ) );
            }
        }
    }
}

SCENARIO( "LinePositionArray with UINT32_MAX offsets", "[linepositionarray]") {

    std::vector<LineOffset> offsets = {
        4_offset,
        8_offset,
        LineOffset(UINT32_MAX - 10),
        LineOffset( (uint64_t) UINT32_MAX + 10LL ),
        LineOffset( (uint64_t) UINT32_MAX + 30LL ),
        LineOffset( (uint64_t) 2*UINT32_MAX ),
        LineOffset( (uint64_t) 2*UINT32_MAX + 10LL ),
        LineOffset( (uint64_t) 2*UINT32_MAX + 1000LL ),
        LineOffset( (uint64_t) 3*UINT32_MAX )
    };

    GIVEN( "LinePositionArray with long offsets") {
        LinePositionArray line_array;

        for ( const auto& offset: offsets ) {
            line_array.append( offset );
        }

        REQUIRE( line_array.size() == 9_lcount );

        WHEN( "Access items in linear order" ) {
            THEN( "Corrent offsets returned") {
                for ( auto i = 0u; i<offsets.size(); ++i ) {
                    REQUIRE( line_array[i] == offsets[i] );
                }
            }
        }

        WHEN( "Adding lines after fake lf") {
            THEN( "Correct offset is returned" )
            for ( uint64_t i = 0; i < 1000; ++i ) {
                uint64_t pos = 3LL*UINT32_MAX + 524LL + i*35LL;
                line_array.append( LineOffset( pos ) );
                line_array.setFakeFinalLF();
                REQUIRE( line_array[9 + i] == LineOffset( pos ) );
                line_array.append( LineOffset( pos + 21LL ) );
                REQUIRE( line_array[9 + i] == LineOffset( pos + 21LL ) );
            }
        }
    }

    GIVEN( "LinePositionArray with small lines" ) {

        LinePositionArray line_array;
        line_array.append( 4_offset );
        line_array.append( 8_offset );

        WHEN( "Appending large lines" ) {
            FastLinePositionArray other_array;
            other_array.append( LineOffset( (uint64_t) UINT32_MAX + 10LL ) );
            other_array.append( LineOffset( (uint64_t) UINT32_MAX + 30LL ) );

            line_array.append_list( other_array );

            THEN( "Correct offsets are returned" ) {
                REQUIRE( line_array.size() == 4_lcount );

                REQUIRE( line_array[0] == 4_offset );
                REQUIRE( line_array[1] == 8_offset );
                REQUIRE( line_array[2] == LineOffset( (uint64_t)  UINT32_MAX + 10 ) );
                REQUIRE( line_array[3] == LineOffset( (uint64_t)  UINT32_MAX + 30 ) );
            }
        }
    }
}
