package org.clehne.revpi;

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

public class LibDataIo {
	public static <T> void loadLib(Class<T> clazz) {
		final String LIB_JNI_REVPI_DIO = "jni_revpi_dio";
		try {
			System.loadLibrary(LIB_JNI_REVPI_DIO);
		} catch (final UnsatisfiedLinkError e) {
			try {
				loadLibFromJar(clazz, LIB_JNI_REVPI_DIO);
			} catch (final IOException _e) {
				throw new UnsatisfiedLinkError(LIB_JNI_REVPI_DIO);
			}
		}
	}

	private static void copyStream(final InputStream in, final OutputStream out) throws IOException {
		final int BYTE_BUFFER_SIZE = 0x1000;
		final byte[] buffer = new byte[BYTE_BUFFER_SIZE];
		for (int len; (len = in.read(buffer)) != -1;) {
			out.write(buffer, 0, len);
		}
	}

	private static <T> void loadLibFromJar(Class<T> clazz, final String libName) throws IOException {
		Objects.requireNonNull(libName);
		final String fileName = "/lib/lib" + libName + ".so";
		final FileAttribute<Set<PosixFilePermission>> permissions = PosixFilePermissions
				.asFileAttribute(PosixFilePermissions.fromString("rw-------"));
		final Path temp = Files.createTempFile(clazz.getName(), ".so", permissions);
		try {
			try (final InputStream libstream = clazz.getResourceAsStream(fileName)) {
				if (libstream == null) {
					throw new FileNotFoundException("jar:*!" + fileName);
				}
				try (final OutputStream fout = Files.newOutputStream(temp, StandardOpenOption.WRITE,
						StandardOpenOption.TRUNCATE_EXISTING)) {
					copyStream(libstream, fout);
				}
			}
			System.load(temp.toString());
		} finally {
			Files.delete(temp);
		}
	}

}
