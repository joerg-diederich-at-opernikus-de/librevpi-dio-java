package org.clehne.revpi.dataio;

import java.io.Closeable;
import java.io.IOException;


public final class DataInOut  extends LibDataInOut implements Closeable {
	private int _fd;

	public DataInOut() {
		try {
			_fd = _openDIO();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void close() throws IOException {
		_close(_fd);
	}
	
	/**
	 *	sets channel value for data OUT module channel O1..O16 
	 * @param _channel 1-16
	 * @param value  true or false  
	 * @throws IOException in case of any error
	 */
	public void setDataOut(int _channel, boolean _value) throws IOException {
		int val = _setDataOut(_fd, _channel, (_value?1:0));
		if(val < 0) {
			throw new IOException("Error setting Data OUT " + _channel + " retCode " + val);
		}
	}
	
	/**
	 *	reads in channel value from data OUT module channel O1..O16 
	 * @param _channel 1-16
	 * @return true or false 
	 * @throws IOException in case of any error
	 */
	public boolean getDataOut(int _channel) throws IOException {
		int val = _getDataOut(_fd, _channel);
		if(val < 0) {
			throw new IOException("Error reading Data OUT " + _channel + " retCode " + val);
		}
		return val == 1;
	}
	
	/**
	 *	reads in channel value from data IN module channel I1..I16 
	 * @param _channel 1-16
	 * @return true or false 
	 * @throws IOException in case of any error
	 */
	public boolean getDataIn(int _channel) throws IOException {
		int val = _getDataIn(_fd, _channel);
		if(val < 0) {
			throw new IOException("Error reading Data IN " + _channel + " retCode " + val);
		}
		return val == 1;
	}
	

	private static native int _openDIO() throws IOException;

	private static native void _close(final int fd) throws IOException;

	private static native int _setDataOut(final int fd, int channel, int value);
	private static native int _getDataOut(final int fd, int channel);
	private static native int _getDataIn(final int fd, int channel);
}
