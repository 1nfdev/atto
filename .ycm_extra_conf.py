RPI_DIR = '/opt/raspberry-pi'
RPI_VCDIR = RPI_DIR+'/raspberry-firmware/hardfp/opt/vc'
RPI_FLAGS = [
    '-I'+RPI_VCDIR+'/include',
    '-I'+RPI_VCDIR+'/include/interface/vcos/pthreads',
    '-I'+RPI_VCDIR+'/include/interface/vmcs_host/linux',
    '-DATTO_PLATFORM_RPI',
    '-std=gnu99']
COMMON_FLAGS = ['-Wall', '-Wextra', '-pedantic', '-std=c99', '-I.', '-I..']

def FlagsForFile(filename, **kwargs):
    flags = COMMON_FLAGS
    if 'app_rpi.c' in filename:
        flags = flags + RPI_FLAGS
    return {'flags': flags}
