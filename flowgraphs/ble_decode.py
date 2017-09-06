#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Ble Decode
# Generated: Wed Sep  6 12:32:16 2017
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from PyQt4.QtCore import QObject, pyqtSlot
from gnuradio import analog
from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import fosphor
from gnuradio import gr
from gnuradio import qtgui
from gnuradio.eng_option import eng_option
from gnuradio.fft import window
from gnuradio.filter import firdes
from gnuradio.qtgui import Range, RangeWidget
from optparse import OptionParser
import ble
import osmosdr
import sip
import sys
import time


class ble_decode(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Ble Decode")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Ble Decode")
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "ble_decode")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.transition_width = transition_width = 300e3
        self.samp_rate = samp_rate = int(4e6)
        self.freq_selector = freq_selector = 2480000000
        self.cutoff_freq = cutoff_freq = 850e3
        self.squelch = squelch = -70
        self.signal_bandwidth = signal_bandwidth = int(1e6)
        self.samp_per_sym = samp_per_sym = 4
        self.lowpass_filter = lowpass_filter = firdes.low_pass(1, samp_rate, cutoff_freq, transition_width, firdes.WIN_HAMMING, 6.76)
        self.label_freq = label_freq = freq_selector/1000000
        self.freq_offset = freq_offset = int(1e6)
        self.channels = channels = { "Ch37": 2402000000, "Ch38": 2426000000, "Ch39": 2480000000}

        ##################################################
        # Blocks
        ##################################################
        self._squelch_range = Range(-100, -30, 1, -70, 200)
        self._squelch_win = RangeWidget(self._squelch_range, self.set_squelch, "squelch", "counter_slider", float)
        self.top_grid_layout.addWidget(self._squelch_win, 0,2,1,1)
        self._freq_selector_options = list(channels.values())
        self._freq_selector_labels = list(channels.keys())
        self._freq_selector_group_box = Qt.QGroupBox('Channel')
        self._freq_selector_box = Qt.QHBoxLayout()
        class variable_chooser_button_group(Qt.QButtonGroup):
            def __init__(self, parent=None):
                Qt.QButtonGroup.__init__(self, parent)
            @pyqtSlot(int)
            def updateButtonChecked(self, button_id):
                self.button(button_id).setChecked(True)
        self._freq_selector_button_group = variable_chooser_button_group()
        self._freq_selector_group_box.setLayout(self._freq_selector_box)
        for i, label in enumerate(self._freq_selector_labels):
        	radio_button = Qt.QRadioButton(label)
        	self._freq_selector_box.addWidget(radio_button)
        	self._freq_selector_button_group.addButton(radio_button, i)
        self._freq_selector_callback = lambda i: Qt.QMetaObject.invokeMethod(self._freq_selector_button_group, "updateButtonChecked", Qt.Q_ARG("int", self._freq_selector_options.index(i)))
        self._freq_selector_callback(self.freq_selector)
        self._freq_selector_button_group.buttonClicked[int].connect(
        	lambda i: self.set_freq_selector(self._freq_selector_options[i]))
        self.top_grid_layout.addWidget(self._freq_selector_group_box, 0,0,1,1)
        self.qtgui_const_sink_x_1 = qtgui.const_sink_c(
        	1024, #size
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_const_sink_x_1.set_update_time(0.1)
        self.qtgui_const_sink_x_1.set_y_axis(-2, 2)
        self.qtgui_const_sink_x_1.set_x_axis(-2, 2)
        self.qtgui_const_sink_x_1.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, "")
        self.qtgui_const_sink_x_1.enable_autoscale(False)
        self.qtgui_const_sink_x_1.enable_grid(False)
        self.qtgui_const_sink_x_1.enable_axis_labels(True)
        
        if not True:
          self.qtgui_const_sink_x_1.disable_legend()
        
        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "red", "red", "red",
                  "red", "red", "red", "red", "red"]
        styles = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        markers = [0, 0, 0, 0, 0,
                   0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_const_sink_x_1.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_const_sink_x_1.set_line_label(i, labels[i])
            self.qtgui_const_sink_x_1.set_line_width(i, widths[i])
            self.qtgui_const_sink_x_1.set_line_color(i, colors[i])
            self.qtgui_const_sink_x_1.set_line_style(i, styles[i])
            self.qtgui_const_sink_x_1.set_line_marker(i, markers[i])
            self.qtgui_const_sink_x_1.set_line_alpha(i, alphas[i])
        
        self._qtgui_const_sink_x_1_win = sip.wrapinstance(self.qtgui_const_sink_x_1.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._qtgui_const_sink_x_1_win, 2,0,1,3)
        self.osmosdr_source_0 = osmosdr.source( args="numchan=" + str(1) + " " + 'hackrf=6f181f' )
        self.osmosdr_source_0.set_sample_rate(samp_rate)
        self.osmosdr_source_0.set_center_freq(freq_selector-freq_offset, 0)
        self.osmosdr_source_0.set_freq_corr(0, 0)
        self.osmosdr_source_0.set_dc_offset_mode(0, 0)
        self.osmosdr_source_0.set_iq_balance_mode(0, 0)
        self.osmosdr_source_0.set_gain_mode(False, 0)
        self.osmosdr_source_0.set_gain(10, 0)
        self.osmosdr_source_0.set_if_gain(20, 0)
        self.osmosdr_source_0.set_bb_gain(20, 0)
        self.osmosdr_source_0.set_antenna('', 0)
        self.osmosdr_source_0.set_bandwidth(0, 0)
          
        self._label_freq_tool_bar = Qt.QToolBar(self)
        
        if "{} MHz".format:
          self._label_freq_formatter = "{} MHz".format
        else:
          self._label_freq_formatter = lambda x: x
        
        self._label_freq_tool_bar.addWidget(Qt.QLabel('Frequency'+": "))
        self._label_freq_label = Qt.QLabel(str(self._label_freq_formatter(self.label_freq)))
        self._label_freq_tool_bar.addWidget(self._label_freq_label)
        self.top_grid_layout.addWidget(self._label_freq_tool_bar, 0,1,1,1)
          
        self.freq_xlating_fir_filter_xxx_0 = filter.freq_xlating_fir_filter_ccc(1, (lowpass_filter), freq_offset, samp_rate)
        self.fosphor_qt_sink_c_0 = fosphor.qt_sink_c()
        self.fosphor_qt_sink_c_0.set_fft_window(window.WIN_BLACKMAN_hARRIS)
        self.fosphor_qt_sink_c_0.set_frequency_range(freq_selector, samp_rate)
        self._fosphor_qt_sink_c_0_win = sip.wrapinstance(self.fosphor_qt_sink_c_0.pyqwidget(), Qt.QWidget)
        self.top_grid_layout.addWidget(self._fosphor_qt_sink_c_0_win, 1,0,1,3)
        self.digital_gmsk_demod_0 = digital.gmsk_demod(
        	samples_per_symbol=samp_per_sym,
        	gain_mu=0.55,
        	mu=0.5,
        	omega_relative_limit=0.035,
        	freq_error=0.0,
        	verbose=False,
        	log=False,
        )
        self.blocks_unpacked_to_packed_xx_0 = blocks.unpacked_to_packed_bb(1, gr.GR_LSB_FIRST)
        self.blocks_file_sink_0 = blocks.file_sink(gr.sizeof_char*1, 'gfsk_out.raw', False)
        self.blocks_file_sink_0.set_unbuffered(False)
        self.ble_decode_0 = ble.decode()
        self.analog_simple_squelch_cc_0_0 = analog.simple_squelch_cc(squelch, 0.1)

        ##################################################
        # Connections
        ##################################################
        self.connect((self.analog_simple_squelch_cc_0_0, 0), (self.digital_gmsk_demod_0, 0))    
        self.connect((self.analog_simple_squelch_cc_0_0, 0), (self.qtgui_const_sink_x_1, 0))    
        self.connect((self.blocks_unpacked_to_packed_xx_0, 0), (self.ble_decode_0, 0))    
        self.connect((self.blocks_unpacked_to_packed_xx_0, 0), (self.blocks_file_sink_0, 0))    
        self.connect((self.digital_gmsk_demod_0, 0), (self.blocks_unpacked_to_packed_xx_0, 0))    
        self.connect((self.freq_xlating_fir_filter_xxx_0, 0), (self.analog_simple_squelch_cc_0_0, 0))    
        self.connect((self.freq_xlating_fir_filter_xxx_0, 0), (self.fosphor_qt_sink_c_0, 0))    
        self.connect((self.osmosdr_source_0, 0), (self.freq_xlating_fir_filter_xxx_0, 0))    

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "ble_decode")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_transition_width(self):
        return self.transition_width

    def set_transition_width(self, transition_width):
        self.transition_width = transition_width
        self.set_lowpass_filter(firdes.low_pass(1, self.samp_rate, self.cutoff_freq, self.transition_width, firdes.WIN_HAMMING, 6.76))

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.set_lowpass_filter(firdes.low_pass(1, self.samp_rate, self.cutoff_freq, self.transition_width, firdes.WIN_HAMMING, 6.76))
        self.osmosdr_source_0.set_sample_rate(self.samp_rate)
        self.fosphor_qt_sink_c_0.set_frequency_range(self.freq_selector, self.samp_rate)

    def get_freq_selector(self):
        return self.freq_selector

    def set_freq_selector(self, freq_selector):
        self.freq_selector = freq_selector
        self._freq_selector_callback(self.freq_selector)
        self.osmosdr_source_0.set_center_freq(self.freq_selector-self.freq_offset, 0)
        self.set_label_freq(self._label_freq_formatter(self.freq_selector/1000000))
        self.fosphor_qt_sink_c_0.set_frequency_range(self.freq_selector, self.samp_rate)

    def get_cutoff_freq(self):
        return self.cutoff_freq

    def set_cutoff_freq(self, cutoff_freq):
        self.cutoff_freq = cutoff_freq
        self.set_lowpass_filter(firdes.low_pass(1, self.samp_rate, self.cutoff_freq, self.transition_width, firdes.WIN_HAMMING, 6.76))

    def get_squelch(self):
        return self.squelch

    def set_squelch(self, squelch):
        self.squelch = squelch
        self.analog_simple_squelch_cc_0_0.set_threshold(self.squelch)

    def get_signal_bandwidth(self):
        return self.signal_bandwidth

    def set_signal_bandwidth(self, signal_bandwidth):
        self.signal_bandwidth = signal_bandwidth

    def get_samp_per_sym(self):
        return self.samp_per_sym

    def set_samp_per_sym(self, samp_per_sym):
        self.samp_per_sym = samp_per_sym

    def get_lowpass_filter(self):
        return self.lowpass_filter

    def set_lowpass_filter(self, lowpass_filter):
        self.lowpass_filter = lowpass_filter
        self.freq_xlating_fir_filter_xxx_0.set_taps((self.lowpass_filter))

    def get_label_freq(self):
        return self.label_freq

    def set_label_freq(self, label_freq):
        self.label_freq = label_freq
        Qt.QMetaObject.invokeMethod(self._label_freq_label, "setText", Qt.Q_ARG("QString", str(self.label_freq)))

    def get_freq_offset(self):
        return self.freq_offset

    def set_freq_offset(self, freq_offset):
        self.freq_offset = freq_offset
        self.osmosdr_source_0.set_center_freq(self.freq_selector-self.freq_offset, 0)
        self.freq_xlating_fir_filter_xxx_0.set_center_freq(self.freq_offset)

    def get_channels(self):
        return self.channels

    def set_channels(self, channels):
        self.channels = channels


def main(top_block_cls=ble_decode, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
