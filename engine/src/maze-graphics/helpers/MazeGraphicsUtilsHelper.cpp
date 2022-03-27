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
#include "maze-core/math/MazeMathAlgebra.hpp"
#include "maze-graphics/MazePixelFormat.hpp"
#include "maze-graphics/MazePixelSheet2D.hpp"


//////////////////////////////////////////
namespace Maze
{
    //////////////////////////////////////////
    namespace GraphicsUtilsHelper
    {
        //////////////////////////////////////////
        MAZE_GRAPHICS_API PixelSheet2D g_asciiSymbolsSheet8x8(Vec2DS(128, 48), PixelFormat::RGBA_U8);

        //////////////////////////////////////////
        MAZE_GRAPHICS_API PixelSheet2D const& GetAsciiSymbolsSheet8x8() { return g_asciiSymbolsSheet8x8;  }

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
        MAZE_GRAPHICS_API void ConstructAsciiSymbolsSheet8x8()
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

            g_asciiSymbolsSheet8x8.setFormat(PixelFormat::RGBA_U8);
            g_asciiSymbolsSheet8x8.setSize(128, 48);

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

                    g_asciiSymbolsSheet8x8.setPixel(px, py, ColorU32(k, k, k, k));

                    if (++py == 48)
                    {
                        ++px;
                        py = 0;
                    }
                }
            }

            g_asciiSymbolsSheet8x8.flipY();
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API PixelSheet2D GenerateSystemFontExtrude(
            PixelSheet2D const& _inSheet,
            S8 columns,
            S8 rows,
            S8 charWidth,
            S8 charHeight)
        {
            S32 const extrude = 1;

            S32 extraPixelsWidth = 2 * extrude * columns;
            S32 extraPixelsHeight = 2 * extrude * rows;

            PixelSheet2D systemFontSheet;
            systemFontSheet.setFormat(PixelFormat::RGBA_U8);
            S32 newWidth = Math::GetNextPowerOfTwo(_inSheet.getWidth() + extraPixelsWidth);
            S32 newHeight = Math::GetNextPowerOfTwo(_inSheet.getHeight() + extraPixelsHeight);
            systemFontSheet.setSize(newWidth, newHeight);

            for (S32 c = 0; c < columns; ++c)
            {
                for (S32 r = 0; r < rows; ++r)
                {
                    S32 sheetCharX = c * (charWidth + extrude * 2) + extrude;
                    S32 sheetCharY = r * (charHeight + extrude * 2) + extrude;

                    S32 packedSheetCharX = c * charWidth;
                    S32 packedSheetCharY = r * charHeight;

                    for (S32 y = 0; y < charHeight; ++y)
                    {
                        for (S32 x = 0; x < charWidth; ++x)
                        {
                            ColorU32 packedSheetCharColor = _inSheet.getPixelRGBA_U8(
                                packedSheetCharX + x,
                                packedSheetCharY + y);
                            systemFontSheet.setPixel(
                                sheetCharX + x,
                                sheetCharY + y,
                                packedSheetCharColor);
                        }
                    }

                    // Horizontal extrude
                    for (S32 y = 0; y < charHeight; ++y)
                    {
                        systemFontSheet.setPixel(sheetCharX - 1, sheetCharY + y,
                            _inSheet.getPixelRGBA_U8(packedSheetCharX, packedSheetCharY + y));

                        systemFontSheet.setPixel(sheetCharX + charWidth, sheetCharY + y,
                            _inSheet.getPixelRGBA_U8(packedSheetCharX + charWidth - 1, packedSheetCharY + y));
                    }

                    // Vertical extrude
                    for (S32 x = 0; x < charWidth; ++x)
                    {
                        systemFontSheet.setPixel(sheetCharX + x, sheetCharY - 1,
                            _inSheet.getPixelRGBA_U8(packedSheetCharX + x, packedSheetCharY));

                        systemFontSheet.setPixel(sheetCharX + x, sheetCharY + charHeight,
                            _inSheet.getPixelRGBA_U8(packedSheetCharX + x, packedSheetCharY + charHeight - 1));
                    }
                }
            }

            return systemFontSheet;
        }

        //////////////////////////////////////////
        MAZE_GRAPHICS_API PixelSheet2D GenerateSystemFontExtrudeOutlined(
            PixelSheet2D const& _inSheet,
            S8 columns,
            S8 rows,
            S8 charWidth,
            S8 charHeight,
            ColorU32 const& _outlineColor)
        {
            S32 const extrude = 1;
            S32 const outline = 1;

            S32 extraPixelsWidth = 2 * (extrude + outline) * columns;
            S32 extraPixelsHeight = 2 * (extrude + outline) * rows;

            PixelSheet2D systemFontSheet;
            systemFontSheet.setFormat(PixelFormat::RGBA_U8);
            S32 newWidth = Math::GetNextPowerOfTwo(_inSheet.getWidth() + extraPixelsWidth);
            S32 newHeight = Math::GetNextPowerOfTwo(_inSheet.getHeight() + extraPixelsHeight);
            systemFontSheet.setSize(newWidth, newHeight);

            for (S32 c = 0; c < columns; ++c)
            {
                for (S32 r = 0; r < rows; ++r)
                {
                    S32 packedSheetCharX = c * charWidth;
                    S32 packedSheetCharY = r * charHeight;

                    for (S32 d = 0; d < 8; d += 1)
                    {
                        S32 ox = 0;
                        S32 oy = 0;

                        switch (d)
                        {
                            case 0: ox += 1; break;
                            case 1: ox -= 1; break;
                            case 2: oy += 1; break;
                            case 3: oy -= 1; break;
                            case 4: ox += 1; oy += 1; break;
                            case 5: ox -= 1; oy += 1; break;
                            case 6: ox += 1; oy -= 1; break;
                            case 7: ox -= 1; oy -= 1; break;
                        }

                        S32 sheetCharX = c * (charWidth + (extrude + outline) * 2) + (extrude + outline) + ox;
                        S32 sheetCharY = r * (charHeight + (extrude + outline) * 2) + (extrude + outline) + oy;

                        for (S32 y = 0; y < charHeight; ++y)
                        {
                            for (S32 x = 0; x < charWidth; ++x)
                            {
                                ColorU32 packedSheetCharColor = _inSheet.getPixelRGBA_U8(
                                    packedSheetCharX + x,
                                    packedSheetCharY + y);

                                if (packedSheetCharColor.a > 0)
                                {
                                    systemFontSheet.setPixel(
                                        sheetCharX + x,
                                        sheetCharY + y,
                                        _outlineColor);
                                }
                            }
                        }
                    }

                    S32 sheetCharX = c * (charWidth + (extrude + outline) * 2) + (extrude + outline);
                    S32 sheetCharY = r * (charHeight + (extrude + outline) * 2) + (extrude + outline);


                    // Horizontal extrude
                    for (S32 y = -outline; y < charHeight + outline; ++y)
                    {
                        systemFontSheet.setPixel(sheetCharX - outline - 1, sheetCharY + y,
                            systemFontSheet.getPixelRGBA_U8(sheetCharX - outline, sheetCharY + y));

                        systemFontSheet.setPixel(sheetCharX + outline + charWidth, sheetCharY + y,
                            systemFontSheet.getPixelRGBA_U8(sheetCharX + outline + charWidth - 1, sheetCharY + y));
                    }
                    
                    // Vertical extrude
                    for (S32 x = -outline; x < charWidth + outline; ++x)
                    {
                        systemFontSheet.setPixel(sheetCharX + x, sheetCharY - outline - 1,
                            systemFontSheet.getPixelRGBA_U8(sheetCharX + x, sheetCharY - outline));

                        systemFontSheet.setPixel(sheetCharX + x, sheetCharY + outline + charHeight,
                            systemFontSheet.getPixelRGBA_U8(sheetCharX + x, sheetCharY + outline + charHeight - 1));
                    }
                }
            }

            for (S32 c = 0; c < columns; ++c)
            {
                for (S32 r = 0; r < rows; ++r)
                {
                    S32 sheetCharX = c * (charWidth + (extrude + outline) * 2) + (extrude + outline);
                    S32 sheetCharY = r * (charHeight + (extrude + outline) * 2) + (extrude + outline);

                    S32 packedSheetCharX = c * charWidth;
                    S32 packedSheetCharY = r * charHeight;

                    for (S32 y = 0; y < charHeight; ++y)
                    {
                        for (S32 x = 0; x < charWidth; ++x)
                        {
                            ColorU32 packedSheetCharColor = _inSheet.getPixelRGBA_U8(
                                packedSheetCharX + x,
                                packedSheetCharY + y);

                            if (packedSheetCharColor.a == 255)
                            {
                                systemFontSheet.setPixel(
                                    sheetCharX + x,
                                    sheetCharY + y,
                                    packedSheetCharColor);
                            }
                        }
                    }
                }
            }

            return systemFontSheet;
        }


    } // namespace GraphicsUtilsHelper
    //////////////////////////////////////////

} // namespace Maze
//////////////////////////////////////////
