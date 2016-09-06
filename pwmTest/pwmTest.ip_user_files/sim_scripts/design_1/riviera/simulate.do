onbreak {quit -force}
onerror {quit -force}

asim -t 1ps +access +r +m+design_1 -L secureip -L xil_defaultlib -O5 xil_defaultlib.design_1

do {wave.do}

view wave
view structure
view signals

do {design_1.udo}

run -all

endsim

quit -force
