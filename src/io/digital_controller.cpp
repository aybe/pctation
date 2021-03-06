#include <io/digital_controller.hpp>

#include <util/types.hpp>

#include <bitset>

namespace io {

u8 DigitalController::read(u8 val) {
  switch (m_read_idx) {
    case 0:
      if (val == 0x01) {  // 01h  Hi-Z  Controller Access (unlike 81h=Memory Card access), dummy response
        advance();
        return 0xFF;  // invalid request
      }
      return 0xFF;
    case 1:
      if (val == 0x42) {  // 42h  idlo  Receive ID bit0..7 (variable) and Send Read Command (ASCII "B")
        advance();
        return 0x41;  // 5A41h=Digital Pad         (or analog pad/stick in digital mode; LED=Off)
      }
      reset();
      return 0xFF;  // invalid request
    case 2: {
      advance();
      return 0x5A;  // TAP  idhi  Receive ID bit8..15 (usually/always 5Ah)
    }
    case 3: {
      advance();
      const auto buttons0 =
          m_buttons.byte[0];  // save buttons to return, since we'll mask the unpressed ones now
      m_buttons.byte[0] |= m_buttons_down_mask.byte[0];  // mask buttons that have been unpressed
      m_buttons_down_mask.byte[0] = 0;                   // reset unpressed mask
      return buttons0;
    }
    case 4: {
      reset();  // reset reading state
      // Do the same as buttons[0], refer to comments above
      const auto buttons1 = m_buttons.byte[1];
      m_buttons.byte[1] |= m_buttons_down_mask.byte[1];
      m_buttons_down_mask.byte[1] = 0;
      return buttons1;
    }
    default: return 0xFF;  // Invalid read index
  }
}

void DigitalController::update_button(u8 button_index, bool was_pressed) {
  if (was_pressed) {
    std::bitset<16> buttons = m_buttons.word;
    buttons.set(button_index, false);
    m_buttons.word = (u16)buttons.to_ulong();
  } else {
    m_buttons_down_mask.word |= 1 << button_index;
  }
}

}  // namespace io
