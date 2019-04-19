


#ifndef _SHIM_WOLFSSL_TYPES_H_
#define _SHIM_WOLFSSL_TYPES_H_



#include <settings.h>

typedef unsigned char  byte;
typedef int  word32;
typedef short  word16;
typedef byte*  buffer;
typedef int wolfSSL_Mutex;

typedef struct ProtocolVersion {
	byte major;
	byte minor;
} ProtocolVersion;

typedef struct WOLFSSL_METHOD {
	ProtocolVersion version;
	byte            side;         /* connection side, server or client */
	byte            downgrade;    /* whether to downgrade version, default no */
} WOLFSSL_METHOD;

/* wolfSSL DER buffer */
typedef struct DerBuffer {
	byte*  buffer;
	void* heap;
	word32 length;
	int type; /* enum CertType */
	int dynType; /* DYNAMIC_TYPE_* */
} DerBuffer;

typedef struct OS_Seed {
  int fd;
} OS_Seed;

typedef struct WC_RNG {
    OS_Seed seed;
    void* heap;
// #ifdef HAVE_HASHDRBG
    /* Hash-based Deterministic Random Bit Generator */
    struct DRBG* drbg;
    byte status;
// #endif
} WC_RNG;

typedef struct DNS_entry   DNS_entry;

struct DNS_entry {
    DNS_entry* next;   /* next on DNS list */
    char*      name;   /* actual DNS name */
};


typedef struct Base_entry  Base_entry;

struct Base_entry {
    Base_entry* next;   /* next on name base list */
    char*       name;   /* actual name base */
    int         nameSz; /* name length */
    byte        type;   /* Name base type (DNS or RFC822) */
};


#define WC_SHA256_DIGEST_SIZE 32
/* CA Signers */
/* if change layout change PERSIST_CERT_CACHE functions too */
typedef struct Signer Signer;
struct Signer {
    word32  pubKeySize;
    word32  keyOID;                  /* key type */
    word16  keyUsage;
    byte    pathLength;
    byte    pathLengthSet;
    byte*   publicKey;
    int     nameLen;
    char*   name;                    /* common name */
#ifndef IGNORE_NAME_CONSTRAINTS
        Base_entry* permittedNames;
        Base_entry* excludedNames;
#endif /* IGNORE_NAME_CONSTRAINTS */
    byte    subjectNameHash[SIGNER_DIGEST_SIZE];
                                     /* sha hash of names in certificate */
    #ifndef NO_SKID
        byte    subjectKeyIdHash[SIGNER_DIGEST_SIZE];
                                     /* sha hash of names in certificate */
    #endif
    Signer* next;
} ;


#ifdef WOLFSSL_TRUST_PEER_CERT
/* used for having trusted peer certs rather then CA */
struct TrustedPeerCert {
    int     nameLen;
    char*   name;                    /* common name */
    #ifndef IGNORE_NAME_CONSTRAINTS
        Base_entry* permittedNames;
        Base_entry* excludedNames;
    #endif /* IGNORE_NAME_CONSTRAINTS */
    byte    subjectNameHash[SIGNER_DIGEST_SIZE];
                                     /* sha hash of names in certificate */
    #ifndef NO_SKID
        byte    subjectKeyIdHash[SIGNER_DIGEST_SIZE];
                                     /* sha hash of names in certificate */
    #endif
    word32 sigLen;
    byte*  sig;
    struct TrustedPeerCert* next;
};
#endif /* WOLFSSL_TRUST_PEER_CERT */


/* wolfSSL Certificate Manager */
struct WOLFSSL_CERT_MANAGER {
    Signer*         caTable[CA_TABLE_SIZE]; /* the CA signer table */
    void*           heap;                /* heap helper */
#ifdef WOLFSSL_TRUST_PEER_CERT
    TrustedPeerCert* tpTable[TP_TABLE_SIZE]; /* table of trusted peer certs */
    wolfSSL_Mutex   tpLock;                  /* trusted peer list lock */
#endif
    WOLFSSL_CRL*    crl;                 /* CRL checker */
    WOLFSSL_OCSP*   ocsp;                /* OCSP checker */
#if !defined(NO_WOLFSSL_SERVER) && (defined(HAVE_CERTIFICATE_STATUS_REQUEST) \
                               ||  defined(HAVE_CERTIFICATE_STATUS_REQUEST_V2))
    WOLFSSL_OCSP*   ocsp_stapling;       /* OCSP checker for OCSP stapling */
#endif
    char*           ocspOverrideURL;     /* use this responder */
    void*           ocspIOCtx;           /* I/O callback CTX */
    CallbackCACache caCacheCallback;     /* CA cache addition callback */
    CbMissingCRL    cbMissingCRL;        /* notify through cb of missing crl */
    CbOCSPIO        ocspIOCb;            /* I/O callback for OCSP lookup */
    CbOCSPRespFree  ocspRespFreeCb;      /* Frees OCSP Response from IO Cb */
    wolfSSL_Mutex   caLock;              /* CA list lock */
    byte            crlEnabled;          /* is CRL on ? */
    byte            crlCheckAll;         /* always leaf, but all ? */
    byte            ocspEnabled;         /* is OCSP on ? */
    byte            ocspCheckAll;        /* always leaf, but all ? */
    byte            ocspSendNonce;       /* send the OCSP nonce ? */
    byte            ocspUseOverrideURL;  /* ignore cert's responder, override */
    byte            ocspStaplingEnabled; /* is OCSP Stapling on ? */

#ifndef NO_RSA
    short           minRsaKeySz;         /* minimum allowed RSA key size */
#endif
#if defined(HAVE_ECC) || defined(HAVE_ED25519)
    short           minEccKeySz;         /* minimum allowed ECC key size */
#endif
};
/* wolfSSL context type */
typedef struct WOLFSSL_CTX {
	WOLFSSL_METHOD* method;
#ifdef SINGLE_THREADED
	WC_RNG*         rng;          /* to be shared with WOLFSSL w/o locking */
#endif
	wolfSSL_Mutex   countMutex;   /* reference count mutex */
	int         refCount;         /* reference count */
	int         err;              /* error code in case of mutex not created */
#ifndef NO_DH
	buffer      serverDH_P;
	buffer      serverDH_G;
#endif
#ifndef NO_CERTS
	DerBuffer*  certificate;
	DerBuffer*  certChain;
	/* chain after self, in DER, with leading size for each cert */
#ifdef OPENSSL_EXTRA
	WOLF_STACK_OF(WOLFSSL_X509_NAME)* ca_names;
#endif
#if defined(WOLFSSL_NGINX) || defined (WOLFSSL_HAPROXY)
	WOLF_STACK_OF(WOLFSSL_X509)* x509Chain;
#endif
#ifdef WOLFSSL_TLS13
	int         certChainCnt;
#endif
	DerBuffer*  privateKey;
	byte        privateKeyType;
	int         privateKeySz;
	WOLFSSL_CERT_MANAGER* cm;      /* our cert manager, ctx owns SSL will use */
#endif
#ifdef KEEP_OUR_CERT
	WOLFSSL_X509*    ourCert;     /* keep alive a X509 struct of cert */
	int              ownOurCert;  /* Dispose of certificate if we own */
#endif
	Suites*     suites;           /* make dynamic, user may not need/set */
	void*       heap;             /* for user memory overrides */
	byte        verifyPeer;
	byte        verifyNone;
	byte        failNoCert;
	byte        failNoCertxPSK;   /* fail if no cert with the exception of PSK*/
	byte        sessionCacheOff;
	byte        sessionCacheFlushOff;
#ifdef HAVE_EXT_CACHE
	byte        internalCacheOff;
#endif
	byte        sendVerify;       /* for client side */
	byte        haveRSA;          /* RSA available */
	byte        haveECC;          /* ECC available */
	byte        haveDH;           /* server DH parms set by user */
	byte        haveNTRU;         /* server private NTRU  key loaded */
	byte        haveECDSAsig;     /* server cert signed w/ ECDSA */
	byte        haveStaticECC;    /* static server ECC private key */
	byte        partialWrite;     /* only one msg per write call */
	byte        quietShutdown;    /* don't send close notify */
	byte        groupMessages;    /* group handshake messages before sending */
	byte        minDowngrade;     /* minimum downgrade version */
	byte        haveEMS;          /* have extended master secret extension */
	byte        useClientOrder:1; /* Use client's cipher preference order */
#ifdef WOLFSSL_TLS13
	byte        noTicketTls13:1;  /* Server won't create new Ticket */
	byte        noPskDheKe:1;     /* Don't use (EC)DHE with PSK */
#endif
#if defined(WOLFSSL_TLS13) && defined(WOLFSSL_POST_HANDSHAKE_AUTH)
	byte        postHandshakeAuth:1;  /* Post-handshake auth supported. */
#endif
#ifdef WOLFSSL_MULTICAST
	byte        haveMcast;        /* multicast requested */
	byte        mcastID;          /* multicast group ID */
#endif
#if defined(WOLFSSL_SCTP) && defined(WOLFSSL_DTLS)
	byte        dtlsSctp;         /* DTLS-over-SCTP mode */
	word16      dtlsMtuSz;        /* DTLS MTU size */
#endif
#ifndef NO_DH
	word16      minDhKeySz;       /* minimum DH key size */
#endif
#ifndef NO_RSA
	short       minRsaKeySz;      /* minimum RSA key size */
#endif
#if defined(HAVE_ECC) || defined(HAVE_ED25519)
	short       minEccKeySz;      /* minimum ECC key size */
#endif
#ifdef OPENSSL_EXTRA
	word32            disabledCurves;   /* curves disabled by user */
	byte              verifyDepth;      /* maximum verification depth */
	unsigned long     mask;             /* store SSL_OP_ flags */
#endif
	CallbackIORecv CBIORecv;
	CallbackIOSend CBIOSend;
#ifdef WOLFSSL_DTLS
	CallbackGenCookie CBIOCookie;       /* gen cookie callback */
	wc_dtls_export    dtls_export;      /* export function for DTLS session */
#ifdef WOLFSSL_SESSION_EXPORT
	CallbackGetPeer CBGetPeer;
	CallbackSetPeer CBSetPeer;
#endif
#endif /* WOLFSSL_DTLS */
	VerifyCallback  verifyCallback;     /* cert verification callback */
	word32          timeout;            /* session timeout */
#if defined(HAVE_ECC) || defined(HAVE_CURVE25519)
	word32          ecdhCurveOID;       /* curve Ecc_Sum */
#endif
#ifdef HAVE_ECC
	word16          eccTempKeySz;       /* in octets 20 - 66 */
	word32          pkCurveOID;         /* curve Ecc_Sum */
#endif
#if defined(HAVE_SESSION_TICKET) || !defined(NO_PSK)
	byte        havePSK;                /* psk key set by user */
	wc_psk_client_callback client_psk_cb;  /* client callback */
	wc_psk_server_callback server_psk_cb;  /* server callback */
	char        server_hint[MAX_PSK_ID_LEN + NULL_TERM_LEN];
#endif /* HAVE_SESSION_TICKET || !NO_PSK */
#ifdef WOLFSSL_EARLY_DATA
	word32          maxEarlyDataSz;
#endif
#ifdef HAVE_ANON
	byte        haveAnon;               /* User wants to allow Anon suites */
#endif /* HAVE_ANON */
#if defined(OPENSSL_EXTRA) || defined(HAVE_WEBSERVER)
	pem_password_cb* passwd_cb;
	void*           userdata;
	WOLFSSL_X509_STORE x509_store; /* points to ctx->cm */
	byte            readAhead;
	void*           userPRFArg; /* passed to prf callback */
#endif /* OPENSSL_EXTRA */
#ifdef HAVE_EX_DATA
	void*           ex_data[MAX_EX_DATA];
#endif
#if defined(HAVE_ALPN) && (defined(WOLFSSL_NGINX) || defined(WOLFSSL_HAPROXY))
	CallbackALPNSelect alpnSelect;
	void*              alpnSelectArg;
#endif
#if defined(HAVE_STUNNEL) || defined(WOLFSSL_NGINX) || defined(WOLFSSL_HAPROXY)
	CallbackSniRecv sniRecvCb;
	void*           sniRecvCbArg;
#endif
#if defined(WOLFSSL_MULTICAST) && defined(WOLFSSL_DTLS)
	CallbackMcastHighwater mcastHwCb; /* Sequence number highwater callback */
	word32      mcastFirstSeq;    /* first trigger level */
	word32      mcastSecondSeq;   /* second tigger level */
	word32      mcastMaxSeq;      /* max level */
#endif
#ifdef HAVE_OCSP
	WOLFSSL_OCSP      ocsp;
#endif
	int             devId;              /* async device id to use */
#ifdef HAVE_TLS_EXTENSIONS
	TLSX* extensions;                  /* RFC 6066 TLS Extensions data */
#ifndef NO_WOLFSSL_SERVER
#if defined(HAVE_CERTIFICATE_STATUS_REQUEST) \
	|| defined(HAVE_CERTIFICATE_STATUS_REQUEST_V2)
	OcspRequest* certOcspRequest;
#endif
#if defined(HAVE_CERTIFICATE_STATUS_REQUEST_V2)
	OcspRequest* chainOcspRequest[MAX_CHAIN_DEPTH];
#endif
#endif
#if defined(HAVE_SESSION_TICKET) && !defined(NO_WOLFSSL_SERVER)
	SessionTicketEncCb ticketEncCb;   /* enc/dec session ticket Cb */
	void*              ticketEncCtx;  /* session encrypt context */
	int                ticketHint;    /* ticket hint in seconds */
#endif
#ifdef HAVE_SUPPORTED_CURVES
	byte userCurves;                  /* indicates user called wolfSSL_CTX_UseSupportedCurve */
#endif
#endif
#ifdef ATOMIC_USER
	CallbackMacEncrypt    MacEncryptCb;    /* Atomic User Mac/Encrypt Cb */
	CallbackDecryptVerify DecryptVerifyCb; /* Atomic User Decrypt/Verify Cb */
#endif
#ifdef HAVE_PK_CALLBACKS
#ifdef HAVE_ECC
	CallbackEccSign   EccSignCb;    /* User EccSign   Callback handler */
	CallbackEccVerify EccVerifyCb;  /* User EccVerify Callback handler */
	CallbackEccSharedSecret EccSharedSecretCb;     /* User EccVerify Callback handler */
#ifdef HAVE_ED25519
	/* User Ed25519Sign   Callback handler */
	CallbackEd25519Sign   Ed25519SignCb;
	/* User Ed25519Verify Callback handler */
	CallbackEd25519Verify Ed25519VerifyCb;
#endif
#ifdef HAVE_CURVE25519
	/* User EccSharedSecret Callback handler */
	CallbackX25519SharedSecret X25519SharedSecretCb;
#endif
#endif /* HAVE_ECC */
#ifndef NO_RSA
	CallbackRsaSign   RsaSignCb;    /* User RsaSign   Callback handler */
	CallbackRsaVerify RsaVerifyCb;  /* User RsaVerify Callback handler */
#ifdef WC_RSA_PSS
	CallbackRsaPssSign   RsaPssSignCb;    /* User RsaPssSign */
	CallbackRsaPssVerify RsaPssVerifyCb;  /* User RsaPssVerify */
#endif
	CallbackRsaEnc    RsaEncCb;     /* User Rsa Public Encrypt  handler */
	CallbackRsaDec    RsaDecCb;     /* User Rsa Private Decrypt handler */
#endif /* NO_RSA */
#endif /* HAVE_PK_CALLBACKS */
#ifdef HAVE_WOLF_EVENT
	WOLF_EVENT_QUEUE event_queue;
#endif /* HAVE_WOLF_EVENT */
#ifdef HAVE_EXT_CACHE
	WOLFSSL_SESSION*(*get_sess_cb)(WOLFSSL*, unsigned char*, int, int*);
	int (*new_sess_cb)(WOLFSSL*, WOLFSSL_SESSION*);
	void (*rem_sess_cb)(WOLFSSL_CTX*, WOLFSSL_SESSION*);
#endif
#if defined(OPENSSL_EXTRA) && defined(WOLFCRYPT_HAVE_SRP) && !defined(NO_SHA256)
	Srp*  srp;  /* TLS Secure Remote Password Protocol*/
	byte* srp_password;
#endif
} WOLFSSL_CTX;


#endif // _SHIM_WOLFSSL_TYPES_H_
