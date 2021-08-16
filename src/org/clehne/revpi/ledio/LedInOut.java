package org.clehne.revpi.ledio;

import java.io.Closeable;
import java.io.IOException;


public final class LedInOut extends LibLedInOut implements Closeable {
	private int _fd;

	public LedInOut() {
		try {
			_fd = _open();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	@Override
	public void close() throws IOException {
		_close(_fd);
	}
	
	public void setBit(int bit, boolean value) throws IOException {
		int val = _setBit(_fd, bit, (value?1:0));
		if(val < 0) {
			throw new IOException("Error setting bit " + bit + " retCode " + val);
		}
	}
	
	public boolean getBit(int bit) throws IOException {
		int val = _getBit(_fd, bit);
		if(val < 0) {
			throw new IOException("Error reading bit " + bit + " retCode " + val);
		}
		return (val == 1);
	}
	
	public void setValue(int argVal) throws IOException {
		int val = _setValue(_fd, argVal);
		if(val < 0) {
			throw new IOException("Error setting value " + argVal + " retCode " + val);
		}
	}
	
	public int getValue() throws IOException {
		long val = _getValue(_fd);
		if(val < 0) {
			throw new IOException("Error getting value, retCode " + val);
		}
		return Long.valueOf(val).intValue();
	}
	

	private static native int _open() throws IOException;
	private static native void _close(final int fd) throws IOException;

	private static native int _setValue(final int fd, int value);
	private static native long _getValue(final int fd);
	private static native int _setBit(final int fd, int bit, int value);
	private static native int _getBit(final int fd, int bit);
}