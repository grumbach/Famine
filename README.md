# Famine

A packed Virus for Elf64 binaries

```
  Infected program memory overview
  ================================

                          .        .
                          |  ...   |
              p_offset -> |========| PT_LOAD  - relative_pt_load_address
                          |  ...   |          ^
                          |  ...   |          |
             sh_offset -> |--------|          |
                      |   |  ...   |          |
                      V   |  ...   |          |
    offset_in_section -   |>>>>>>>>| entry    |- relative_entry_address
                          |  ...   |          |^
                          |--------|          ||
                          |  ...   |          ||
                          |  ...   |          ||
   end_of_last_section -> |--------| -  -  -  -- -  -  -  -  -  -
         @famine_entry -> |@@@@@@@@| |                          |
                          |@      @| |                          |
                          |@      @| |                          |
                          |@@@@@@@@| V                          |
         packed @virus -> |~~~~~~~~| - relative_virus_addresss  |
                          |########| |                          |
                          |########| |                          |
                          |########| |                          |
                          |########| V                          V
 @_start (placeholder) -> |~~~~~~~~| - virus_size               - payload_size
                          |  ...   |
                          |  ...   |
                          |========|
                          |  ...   |
                          .        .

 Note that relative_virus_addresss is in the opposite direction !
```
