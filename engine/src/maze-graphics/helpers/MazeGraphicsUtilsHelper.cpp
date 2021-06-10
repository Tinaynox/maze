//////////////////////////////////////////
//
// Maze Engine
// Copyright (C) 2021 Dmitriy "Tinaynox" Nosov (tinaynox@gmail.com)
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it freely,
// subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
//    you must not claim that you wrote the original software.
//    If you use this software in a product, an acknowledgment
//    in the product documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such,
//    and must not be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.
//
//////////////////////////////////////////


//////////////////////////////////////////
#include "MazeGraphicsHeader.hpp"
#include "maze-graphics/loaders/texture/MazeLoaderPNG.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include <png.h>


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace GraphicsUtilsHelper
    {
        //////////////////////////////////////////
        MAZE_GRAPHICS_API PixelSheet2D g_systemFontSheet(Vec2DS(128, 48), PixelFormat::RGBA_U8);

        //////////////////////////////////////////
        MAZE_GRAPHICS_API PixelSheet2D const& GetSystemFontSheet() { return g_systemFontSheet;  }

        //////////////////////////////////////////
        // Tables of ASCII symbols (code symbols - 32-127)
        // Symbol size: 8x8 px
        // |  !"#$%&'()*+,-./ |
        // | 0123456789:;<=>? |
        // | @ABCDEFGHIJKLMNO |
        // | PQRSTUVWXYZ[\]^_ |
        // | `abcdefghijklmno |
        // | pqrstuvwxyz{|}~^ |
        //////////////////////////////////////////
        MAZE_GRAPHICS_API void ConstructSystemFont()
        {
            String data;
            data += "?Q`0001oOch0o01o@F40o0<AGD4090LAGD<090@A7ch0?00O7Q`0600>00000000";
            data += "O000000nOT0063Qo4d8>?7a14Gno94AA4gno94AaOT0>o3`oO400o7QN00000400";
            data += "Of80001oOg<7O7moBGT7O7lABET024@aBEd714AiOdl717a_=TH013Q>00000000";
            data += "720D000V?V5oB3Q_HdUoE7a9@DdDE4A9@DmoE4A;Hg]oM4Aj8S4D84@`00000000";
            data += "OaPT1000Oa`^13P1@AI[?g`1@A=[OdAoHgljA4Ao?WlBA7l1710007l100000000";
            data += "ObM6000oOfMV?3QoBDD`O7a0BDDH@5A0BDD<@5A0BGeVO5ao@CQR?5Po00000000";
            data += "Oc``000?Ogij70PO2D]??0Ph2DUM@7i`2DTg@7lh2GUj?0TO0C1870T?00000000";
            data += "70<4001o?P<7?1QoHg43O;`h@GT0@:@LB@d0>:@hN@L0@?aoN@<0O7ao0000?000";
            data += "OcH0001SOglLA7mg24TnK7ln24US>0PL24U140PnOgl0>7QgOcH0K71S0000A000";
            data += "00H00000@Dm1S007@DUSg00?OdTnH7YhOfTL<7Yh@Cl0700?@Ah0300700000000";
            data += "<008001QL00ZA41a@6HnI<1i@FHLM81M@@0LG81?O`0nC?Y7?`0ZA7Y300080000";
            data += "O`082000Oh0827mo6>Hn?Wmo?6HnMb11MP08@C11H`08@FP0@@0004@000000000";
            data += "00P00001Oab00003OcKP0006@6=PMgl<@440MglH@000000`@000001P00000000";
            data += "Ob@8@@00Ob@8@Ga13R@8Mga172@8?PAo3R@827QoOb@820@0O`0007`0000007P0";
            data += "O`000P08Od400g`<3V=P0G`673IP0`@3>1`00P@6O`P00g`<O`000GP800000000";
            data += "?P9PL020O`<`N3R0@E4HC7b0@ET<ATB0@@l6C4B0O`H3N7b0?P01L3R000000020";

            g_systemFontSheet.setFormat(PixelFormat::RGBA_U8);
            g_systemFontSheet.setSize(128, 48);

            S32 px = 0;
            S32 py = 0;

            for (S32 b = 0; b < 1024; b += 4)
            {
                U32 sym1 = (U32)data[(Size)b + 0] - 48;
                U32 sym2 = (U32)data[(Size)b + 1] - 48;
                U32 sym3 = (U32)data[(Size)b + 2] - 48;
                U32 sym4 = (U32)data[(Size)b + 3] - 48;
                U32 r = sym1 << 18 | sym2 << 12 | sym3 << 6 | sym4;

                for (S32 i = 0; i < 24; ++i)
                {
                    U8 k = r & (1 << i) ? 255 : 0;

                    g_systemFontSheet.setPixel(px, py, ColorU32(k, k, k, k));

                    if (++py == 48)
                    {
                        ++px;
                        py = 0;
                    }
                }
            }

            g_systemFontSheet.flipY();
        }

    } // namespace GraphicsUtilsHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
