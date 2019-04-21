#pragma once

#include <util/types.hpp>

namespace bios {
class Bios;
}
namespace memory {
class Ram;
class Scratchpad;
class Dma;
}  // namespace memory

namespace gpu {
class Gpu;
}

namespace spu {
class Spu;
}

namespace bus {

class Bus {
 public:
  explicit Bus(bios::Bios const& bios,
               memory::Scratchpad& scratchpad,
               memory::Ram& ram,
               memory::Dma& dma,
               gpu::Gpu& gpu,
               spu::Spu& spu)
      : m_ram(ram),
        m_scratchpad(scratchpad),
        m_bios(bios),
        m_dma(dma),
        m_gpu(gpu),
        m_spu(spu) {}
  u32 read32(u32 addr) const;
  u16 read16(u32 addr) const;
  u8 read8(u32 addr) const;
  void write32(u32 addr, u32 val);
  void write16(u32 addr, u16 val);
  void write8(u32 addr, u8 val);

  // TODO: Move elsewhere
  // Interrupts
  u32 m_istat{};
  u32 m_imask{};

  memory::Ram& m_ram;

 private:
  memory::Scratchpad& m_scratchpad;
  bios::Bios const& m_bios;
  memory::Dma& m_dma;
  gpu::Gpu& m_gpu;
  spu::Spu& m_spu;
};

}  // namespace bus
