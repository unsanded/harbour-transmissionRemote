.pragma library

function prettyFileSize(bytes) {
    var number;
    var suffix;
    if(bytes>  Math.pow(2,30)){
        number = (bytes/ Math.pow(2,30));
        suffix = "GiB";
    }
    else
    if(bytes>  Math.pow(2,20)){
        number = (bytes/ Math.pow(2,20)) ;
        suffix = "MiB";
    }
    else
    if(bytes>  Math.pow(2,10)){
        number = (bytes/ Math.pow(2,10))
        suffix = "KiB";
    }
    else
    {
        number = bytes
        suffix = "B"
    }
    return ("" + number).substr(0,3) + suffix;
}
