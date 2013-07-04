#include <ldns/ldns.h>

static int
usage(FILE *fp, char *prog) {
  fprintf(fp, "%s domain\n", prog);
  return 0;
}

ldns_rr_list* dnsutils_retrieve_rr_lists(ldns_pkt *p, ldns_rr_type t, ldns_pkt_section s) {
  ldns_rr_list *tmp = ldns_pkt_rr_list_by_type(p, t, s);

  if (!tmp) {
    ldns_pkt_free(p);
  } else {
    ldns_rr_list_sort(tmp);
  }
  return tmp;
}


// A, AAAA, MX records 
ldns_rr_list** dnsutils_complete_questioning(ldns_resolver *res, ldns_rdf *domain) {
    
    ldns_rr_list** all_records = malloc(3 * sizeof(ldns_rr_list));
    ldns_rr_type wanted_types[3] = { LDNS_RR_TYPE_A, LDNS_RR_TYPE_AAAA, LDNS_RR_TYPE_MX };

    if(!all_records) {
	return NULL;
    }

    for (int i = 0; i < 3; ++i) {

	ldns_pkt *p = NULL;

	p = ldns_resolver_query(res,
				domain,
				wanted_types[i],
				LDNS_RR_CLASS_IN,
				LDNS_RD);

	if(!p) {
	    // TODO: correct error handling
	    all_records[i] = NULL;
	} else {
	    all_records[i] = dnsutils_retrieve_rr_lists(p,
							wanted_types[i],
							LDNS_SECTION_ANSWER);
 	}		
    }
    
    return all_records;
}

int
main(int argc, char *argv[])
{
    ldns_resolver *res;
    ldns_rdf *domain;
    ldns_pkt *p;
    ldns_rr_list *mx, *a, *aaaa;
    ldns_status s;

    p = NULL;
    mx = NULL, a = NULL, aaaa = NULL;
    domain = NULL;
    res = NULL;

    if (argc != 2) {
	usage(stdout, argv[0]);
	exit(EXIT_FAILURE);
    } else {
	/* create a rdf from the command line arg */
	domain = ldns_dname_new_frm_str(argv[1]);
	if (!domain) {
	    usage(stdout, argv[0]);
	    exit(EXIT_FAILURE);
	}
    }

    /* create a new resolver from /etc/resolv.conf */
    s = ldns_resolver_new_frm_file(&res, NULL);

    if (s != LDNS_STATUS_OK) {
	exit(EXIT_FAILURE);
    }

    ldns_rr_list** results = dnsutils_complete_questioning(res,
							   domain);

    for (int i = 0; i < 3; ++i)
    {
	for(int j = 0; j < ldns_rr_list_rr_count(results[i]) && results[i]; ++j) {
	    ldns_rr* rr = ldns_rr_list_rr(results[i], j);

	    ldns_rdf* rdf = ldns_rr_pop_rdf(rr);

	    switch(ldns_rdf_get_type(rdf)) {
	    case LDNS_RDF_TYPE_A:
		printf ("%s has address %s\n", argv[1], ldns_rdf2str(rdf));
		break;
	    case LDNS_RDF_TYPE_AAAA:
		printf ("%s has IPv6 address %s\n", argv[1], ldns_rdf2str(rdf));
		break;
	    case LDNS_RDF_TYPE_DNAME:
		printf ("%s mail is handled by %d %s\n", argv[1], ldns_rdf2native_int16(ldns_rr_mx_preference(rr)), ldns_rdf2str(rdf));
		break;
	    default:
		printf ("%s\n", "something different");
		break;
	    }
	    ldns_rdf_deep_free(rdf);
	}
    }

    ldns_pkt_free(p);
    ldns_resolver_deep_free(res);
    return 0;
}
