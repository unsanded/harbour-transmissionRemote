.pragma library

function prettyFileSize(bytes) {
    if(bytes>  Math.pow(2,30)){
        return (bytes/ Math.pow(2,30)) + "GiB";
    }
    if(bytes>  Math.pow(2,20)){
        return (bytes/ Math.pow(2,20)) + "MiB";
    }
    if(bytes>  Math.pow(2,10)){
        return (bytes/ Math.pow(2,10)) + "KiB";
    }
    return bytes + "B"
}
