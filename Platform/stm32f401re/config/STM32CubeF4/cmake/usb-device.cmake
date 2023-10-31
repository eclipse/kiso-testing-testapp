get_filename_component(USB_BASE_PATH "${stm32cubef4_SOURCE_DIR}/Middlewares/ST/STM32_USB_Device_Library" REALPATH)

if(STM32CUBEF4_USBD_CLASS)
  if(${STM32CUBEF4_USBD_CLASS} STREQUAL "AUDIO")
    set(USB_CLASS_FILES ${USB_BASE_PATH}/Class/AUDIO/Src/usbd_audio.c)
    set(USB_CLASS_INC ${USB_BASE_PATH}/Class/AUDIO/Inc)
  elseif(${STM32CUBEF4_USBD_CLASS} STREQUAL "CDC")
    set(USB_CLASS_FILES ${USB_BASE_PATH}/Class/CDC/Src/usbd_cdc.c)
    set(USB_CLASS_INC ${USB_BASE_PATH}/Class/CDC/Inc)
  elseif(${STM32CUBEF4_USBD_CLASS} STREQUAL "DFU")
    set(USB_CLASS_FILES ${USB_BASE_PATH}/Class/DFU/Src/usbd_dfu.c)
    set(USB_CLASS_INC ${USB_BASE_PATH}/Class/DFU/Inc)
  elseif(${STM32CUBEF4_USBD_CLASS} STREQUAL "HID")
    set(USB_CLASS_FILES ${USB_BASE_PATH}/Class/HID/Src/usbd_hid.c)
    set(USB_CLASS_INC ${USB_BASE_PATH}/Class/HID/Inc)
  elseif(${STM32CUBEF4_USBD_CLASS} STREQUAL "MSC")
    set(USB_CLASS_FILES 
      ${USB_BASE_PATH}/Class/MSC/Src/usbd_msc_bot.c
      ${USB_BASE_PATH}/Class/MSC/Src/usbd_msc_data.c
      ${USB_BASE_PATH}/Class/MSC/Src/usbd_msc_scsi.c
      ${USB_BASE_PATH}/Class/MSC/Src/usbd_msc.c
      )
    set(USB_CLASS_INC ${USB_BASE_PATH}/Class/MSC/Inc)
  endif()
endif()

if(CMAKE_SYSTEM_PROCESSOR STREQUAL "arm")

  add_library(stm32cubef4-usb-device
    ${USB_CLASS_FILES}
    ${USB_BASE_PATH}/Core/Src/usbd_core.c
    ${USB_BASE_PATH}/Core/Src/usbd_ctlreq.c
    ${USB_BASE_PATH}/Core/Src/usbd_ioreq.c
    )

  target_include_directories(stm32cubef4-usb-device
    PUBLIC
      ${STM32CUBEF4_CONF}
      ${USB_CLASS_INC}
      ${USB_BASE_PATH}/Core/Inc
    )

  target_link_libraries(stm32cubef4-usb-device
    PUBLIC
      # The config file (usbd_conf.h) most likely needs to access HAL functions,
      # thus even if the library's source file itself does not have this dependency,
      # this link library needs to be added.
      stm32cubef4-hal
    )

else()

  add_library(stm32cubef4-usb-device INTERFACE
    )

  target_include_directories(stm32cubef4-usb-device
  INTERFACE
    ${STM32CUBEF4_CONF}
    ${USB_CLASS_INC}
    ${USB_BASE_PATH}/Core/Inc
  )

  target_link_libraries(stm32cubef4-usb-device
    INTERFACE
      # The config file (usbd_conf.h) most likely needs to access HAL functions,
      # thus even if the library's source file itself does not have this dependency,
      # this link library needs to be added.
      stm32cubef4-hal
    )

endif()
