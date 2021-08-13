package org.clehne.revpi.ledio;

import org.clehne.revpi.LibDataIo;

public class LibLedInOut extends LibDataIo{
		static {
			LibDataIo.loadLib(LedInOut.class);
		}
}
