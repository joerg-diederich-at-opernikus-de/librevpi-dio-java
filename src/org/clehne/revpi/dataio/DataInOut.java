package org.clehne.revpi.dataio;

//Taken from: https://github.com/entropia/libsocket-can-java

import java.io.Closeable;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.StandardOpenOption;
import java.nio.file.attribute.FileAttribute;
import java.nio.file.attribute.PosixFilePermission;
import java.nio.file.attribute.PosixFilePermissions;
import java.util.Objects;
import java.util.Set;

//import org.apache.log4j.Logger;

public final class DataInOut implements Closeable {
//	private static Logger log = Logger.getLogger(DataInOut.class);
	static {
//		System.out.println("Trying to load native library revpi.dataio");
		final String LIB_JNI_REVPI_DIO = "jni_revpi_dio";
		try {
//			System.out.println("Try loadLibrary");
			System.loadLibrary(LIB_JNI_REVPI_DIO);
		} catch (final UnsatisfiedLinkError e) {
			try {
//				System.out.println("Try load from JAR");
				loadLibFromJar(LIB_JNI_REVPI_DIO);
			} catch (final IOException _e) {
//				System.out.println("ERROR: Cannot load CanSocket native library");
				throw new UnsatisfiedLinkError(LIB_JNI_REVPI_DIO);
			}
		}
//		System.out.println("Succesfully loaded native library revpi.dataio");
	}

	private static void copyStream(final InputStream in, final OutputStream out) throws IOException {
		final int BYTE_BUFFER_SIZE = 0x1000;
		final byte[] buffer = new byte[BYTE_BUFFER_SIZE];
		for (int len; (len = in.read(buffer)) != -1;) {
			out.write(buffer, 0, len);
		}
	}

	private static void loadLibFromJar(final String libName) throws IOException {
		Objects.requireNonNull(libName);
		final String fileName = "/lib/lib" + libName + ".so";
//		System.out.println("Load from JAR: " + fileName);
		final FileAttribute<Set<PosixFilePermission>> permissions = PosixFilePermissions
				.asFileAttribute(PosixFilePermissions.fromString("rw-------"));
		final Path tempSo = Files.createTempFile(DataInOut.class.getName(), ".so", permissions);
		try {
			try (final InputStream libstream = DataInOut.class.getResourceAsStream(fileName)) {
				if (libstream == null) {
					throw new FileNotFoundException("jar:*!" + fileName);
				}
				try (final OutputStream fout = Files.newOutputStream(tempSo, StandardOpenOption.WRITE,
						StandardOpenOption.TRUNCATE_EXISTING)) {
					copyStream(libstream, fout);
				}
			}
			System.load(tempSo.toString());
		} finally {
			Files.delete(tempSo);
		}
	}

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