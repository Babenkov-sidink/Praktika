--
-- PostgreSQL database dump
--

\restrict lywVrlHm07U16Ct4kTYSi5DNALEcg7lRI4IA0GoPHs3e8iphvbCl5J0zlRu4Mof

-- Dumped from database version 18.0
-- Dumped by pg_dump version 18.0

-- Started on 2026-04-21 19:25:18

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET transaction_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 231 (class 1255 OID 43695)
-- Name: next_order_number(); Type: FUNCTION; Schema: public; Owner: postgre
--

CREATE FUNCTION public.next_order_number() RETURNS character varying
    LANGUAGE plpgsql
    AS $$
BEGIN
    RETURN 'ORD-' || LPAD(nextval('order_number_seq')::TEXT, 3, '0');
END;
$$;


ALTER FUNCTION public.next_order_number() OWNER TO postgre;

--
-- TOC entry 230 (class 1255 OID 27180)
-- Name: update_total_cost(); Type: FUNCTION; Schema: public; Owner: postgre
--

CREATE FUNCTION public.update_total_cost() RETURNS trigger
    LANGUAGE plpgsql
    AS $$
BEGIN
    NEW.total_cost := (SELECT base_price FROM work_types WHERE id = NEW.work_type_id) + 
                      (NEW.hours_spent * (SELECT hourly_rate FROM performers WHERE id = NEW.performer_id));
    RETURN NEW;
END;
$$;


ALTER FUNCTION public.update_total_cost() OWNER TO postgre;

SET default_tablespace = '';

SET default_table_access_method = heap;

--
-- TOC entry 222 (class 1259 OID 35393)
-- Name: customers; Type: TABLE; Schema: public; Owner: postgre
--

CREATE TABLE public.customers (
    id integer NOT NULL,
    user_id integer,
    full_name character varying(100) NOT NULL,
    phone character varying(20) NOT NULL,
    email character varying(100),
    address text
);


ALTER TABLE public.customers OWNER TO postgre;

--
-- TOC entry 221 (class 1259 OID 35392)
-- Name: customers_id_seq; Type: SEQUENCE; Schema: public; Owner: postgre
--

CREATE SEQUENCE public.customers_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.customers_id_seq OWNER TO postgre;

--
-- TOC entry 4952 (class 0 OID 0)
-- Dependencies: 221
-- Name: customers_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgre
--

ALTER SEQUENCE public.customers_id_seq OWNED BY public.customers.id;


--
-- TOC entry 229 (class 1259 OID 43694)
-- Name: order_number_seq; Type: SEQUENCE; Schema: public; Owner: postgre
--

CREATE SEQUENCE public.order_number_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.order_number_seq OWNER TO postgre;

--
-- TOC entry 226 (class 1259 OID 35425)
-- Name: performers; Type: TABLE; Schema: public; Owner: postgre
--

CREATE TABLE public.performers (
    id integer NOT NULL,
    user_id integer,
    full_name character varying(100) NOT NULL,
    specialty character varying(100) NOT NULL,
    phone character varying(20),
    hourly_rate numeric(10,2) NOT NULL,
    CONSTRAINT performers_hourly_rate_check CHECK ((hourly_rate >= (0)::numeric))
);


ALTER TABLE public.performers OWNER TO postgre;

--
-- TOC entry 225 (class 1259 OID 35424)
-- Name: performers_id_seq; Type: SEQUENCE; Schema: public; Owner: postgre
--

CREATE SEQUENCE public.performers_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.performers_id_seq OWNER TO postgre;

--
-- TOC entry 4953 (class 0 OID 0)
-- Dependencies: 225
-- Name: performers_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgre
--

ALTER SEQUENCE public.performers_id_seq OWNED BY public.performers.id;


--
-- TOC entry 228 (class 1259 OID 35444)
-- Name: repair_orders; Type: TABLE; Schema: public; Owner: postgre
--

CREATE TABLE public.repair_orders (
    id integer NOT NULL,
    order_number character varying(50) NOT NULL,
    customer_id integer NOT NULL,
    work_type_id integer NOT NULL,
    performer_id integer NOT NULL,
    order_date date DEFAULT CURRENT_DATE NOT NULL,
    deadline_date date NOT NULL,
    status character varying(30) DEFAULT 'в работе'::character varying,
    hours_spent numeric(5,2) DEFAULT 0,
    total_cost numeric(10,2),
    description text,
    CONSTRAINT chk_dates CHECK ((deadline_date >= order_date))
);


ALTER TABLE public.repair_orders OWNER TO postgre;

--
-- TOC entry 227 (class 1259 OID 35443)
-- Name: repair_orders_id_seq; Type: SEQUENCE; Schema: public; Owner: postgre
--

CREATE SEQUENCE public.repair_orders_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.repair_orders_id_seq OWNER TO postgre;

--
-- TOC entry 4954 (class 0 OID 0)
-- Dependencies: 227
-- Name: repair_orders_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgre
--

ALTER SEQUENCE public.repair_orders_id_seq OWNED BY public.repair_orders.id;


--
-- TOC entry 220 (class 1259 OID 35373)
-- Name: users; Type: TABLE; Schema: public; Owner: postgre
--

CREATE TABLE public.users (
    id integer NOT NULL,
    username character varying(50) NOT NULL,
    password_hash character varying(255) NOT NULL,
    full_name character varying(100) NOT NULL,
    email character varying(100) NOT NULL,
    role character varying(30) DEFAULT 'client'::character varying,
    created_at timestamp without time zone DEFAULT CURRENT_TIMESTAMP
);


ALTER TABLE public.users OWNER TO postgre;

--
-- TOC entry 219 (class 1259 OID 35372)
-- Name: users_id_seq; Type: SEQUENCE; Schema: public; Owner: postgre
--

CREATE SEQUENCE public.users_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.users_id_seq OWNER TO postgre;

--
-- TOC entry 4955 (class 0 OID 0)
-- Dependencies: 219
-- Name: users_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgre
--

ALTER SEQUENCE public.users_id_seq OWNED BY public.users.id;


--
-- TOC entry 224 (class 1259 OID 35412)
-- Name: work_types; Type: TABLE; Schema: public; Owner: postgre
--

CREATE TABLE public.work_types (
    id integer NOT NULL,
    name character varying(100) NOT NULL,
    base_price numeric(10,2) NOT NULL,
    CONSTRAINT work_types_base_price_check CHECK ((base_price >= (0)::numeric))
);


ALTER TABLE public.work_types OWNER TO postgre;

--
-- TOC entry 223 (class 1259 OID 35411)
-- Name: work_types_id_seq; Type: SEQUENCE; Schema: public; Owner: postgre
--

CREATE SEQUENCE public.work_types_id_seq
    AS integer
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


ALTER SEQUENCE public.work_types_id_seq OWNER TO postgre;

--
-- TOC entry 4956 (class 0 OID 0)
-- Dependencies: 223
-- Name: work_types_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgre
--

ALTER SEQUENCE public.work_types_id_seq OWNED BY public.work_types.id;


--
-- TOC entry 4751 (class 2604 OID 35396)
-- Name: customers id; Type: DEFAULT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.customers ALTER COLUMN id SET DEFAULT nextval('public.customers_id_seq'::regclass);


--
-- TOC entry 4753 (class 2604 OID 35428)
-- Name: performers id; Type: DEFAULT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.performers ALTER COLUMN id SET DEFAULT nextval('public.performers_id_seq'::regclass);


--
-- TOC entry 4754 (class 2604 OID 35447)
-- Name: repair_orders id; Type: DEFAULT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.repair_orders ALTER COLUMN id SET DEFAULT nextval('public.repair_orders_id_seq'::regclass);


--
-- TOC entry 4748 (class 2604 OID 35376)
-- Name: users id; Type: DEFAULT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.users ALTER COLUMN id SET DEFAULT nextval('public.users_id_seq'::regclass);


--
-- TOC entry 4752 (class 2604 OID 35415)
-- Name: work_types id; Type: DEFAULT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.work_types ALTER COLUMN id SET DEFAULT nextval('public.work_types_id_seq'::regclass);


--
-- TOC entry 4939 (class 0 OID 35393)
-- Dependencies: 222
-- Data for Name: customers; Type: TABLE DATA; Schema: public; Owner: postgre
--

COPY public.customers (id, user_id, full_name, phone, email, address) FROM stdin;
1	2	Алексеев Алексей Алексеевич	+79165678901	alex@mail.ru	ул. Ленина 10
2	3	Смирнова Ольга Владимировна	+79166789012	olga@mail.ru	пр. Мира 25
\.


--
-- TOC entry 4943 (class 0 OID 35425)
-- Dependencies: 226
-- Data for Name: performers; Type: TABLE DATA; Schema: public; Owner: postgre
--

COPY public.performers (id, user_id, full_name, specialty, phone, hourly_rate) FROM stdin;
1	\N	Иванов Иван Иванович	Электронщик	+79161234567	800.00
2	\N	Петров Петр Петрович	Механик	+79162345678	900.00
3	\N	Сидоров Сидор Сидорович	Кузовщик	+79163456789	1000.00
4	\N	Кузнецова Анна Сергеевна	Диагност	+79164567890	750.00
\.


--
-- TOC entry 4945 (class 0 OID 35444)
-- Dependencies: 228
-- Data for Name: repair_orders; Type: TABLE DATA; Schema: public; Owner: postgre
--

COPY public.repair_orders (id, order_number, customer_id, work_type_id, performer_id, order_date, deadline_date, status, hours_spent, total_cost, description) FROM stdin;
13	ORD-001	1	1	1	2026-04-06	2026-04-13	в работе	0.00	500.00	
14	ORD-002	1	1	1	2026-04-06	2026-04-13	в работе	20.00	16500.00	
\.


--
-- TOC entry 4937 (class 0 OID 35373)
-- Dependencies: 220
-- Data for Name: users; Type: TABLE DATA; Schema: public; Owner: postgre
--

COPY public.users (id, username, password_hash, full_name, email, role, created_at) FROM stdin;
1	admin	admin123	Администратор	admin@workshop.ru	admin	2026-04-05 15:32:44.25762
2	alexey	pass123	Алексеев Алексей Алексеевич	alex@mail.ru	client	2026-04-05 15:32:44.25762
3	olga	pass456	Смирнова Ольга Владимировна	olga@mail.ru	client	2026-04-05 15:32:44.25762
\.


--
-- TOC entry 4941 (class 0 OID 35412)
-- Dependencies: 224
-- Data for Name: work_types; Type: TABLE DATA; Schema: public; Owner: postgre
--

COPY public.work_types (id, name, base_price) FROM stdin;
1	Электроника	500.00
2	Механика	700.00
3	Кузовные работы	1000.00
4	Диагностика	300.00
\.


--
-- TOC entry 4957 (class 0 OID 0)
-- Dependencies: 221
-- Name: customers_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgre
--

SELECT pg_catalog.setval('public.customers_id_seq', 2, true);


--
-- TOC entry 4958 (class 0 OID 0)
-- Dependencies: 229
-- Name: order_number_seq; Type: SEQUENCE SET; Schema: public; Owner: postgre
--

SELECT pg_catalog.setval('public.order_number_seq', 10, true);


--
-- TOC entry 4959 (class 0 OID 0)
-- Dependencies: 225
-- Name: performers_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgre
--

SELECT pg_catalog.setval('public.performers_id_seq', 4, true);


--
-- TOC entry 4960 (class 0 OID 0)
-- Dependencies: 227
-- Name: repair_orders_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgre
--

SELECT pg_catalog.setval('public.repair_orders_id_seq', 14, true);


--
-- TOC entry 4961 (class 0 OID 0)
-- Dependencies: 219
-- Name: users_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgre
--

SELECT pg_catalog.setval('public.users_id_seq', 3, true);


--
-- TOC entry 4962 (class 0 OID 0)
-- Dependencies: 223
-- Name: work_types_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgre
--

SELECT pg_catalog.setval('public.work_types_id_seq', 4, true);


--
-- TOC entry 4768 (class 2606 OID 35403)
-- Name: customers customers_pkey; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.customers
    ADD CONSTRAINT customers_pkey PRIMARY KEY (id);


--
-- TOC entry 4770 (class 2606 OID 35405)
-- Name: customers customers_user_id_key; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.customers
    ADD CONSTRAINT customers_user_id_key UNIQUE (user_id);


--
-- TOC entry 4776 (class 2606 OID 35435)
-- Name: performers performers_pkey; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.performers
    ADD CONSTRAINT performers_pkey PRIMARY KEY (id);


--
-- TOC entry 4778 (class 2606 OID 35437)
-- Name: performers performers_user_id_key; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.performers
    ADD CONSTRAINT performers_user_id_key UNIQUE (user_id);


--
-- TOC entry 4780 (class 2606 OID 35464)
-- Name: repair_orders repair_orders_order_number_key; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.repair_orders
    ADD CONSTRAINT repair_orders_order_number_key UNIQUE (order_number);


--
-- TOC entry 4782 (class 2606 OID 35462)
-- Name: repair_orders repair_orders_pkey; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.repair_orders
    ADD CONSTRAINT repair_orders_pkey PRIMARY KEY (id);


--
-- TOC entry 4762 (class 2606 OID 35391)
-- Name: users users_email_key; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_email_key UNIQUE (email);


--
-- TOC entry 4764 (class 2606 OID 35387)
-- Name: users users_pkey; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_pkey PRIMARY KEY (id);


--
-- TOC entry 4766 (class 2606 OID 35389)
-- Name: users users_username_key; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.users
    ADD CONSTRAINT users_username_key UNIQUE (username);


--
-- TOC entry 4772 (class 2606 OID 35423)
-- Name: work_types work_types_name_key; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.work_types
    ADD CONSTRAINT work_types_name_key UNIQUE (name);


--
-- TOC entry 4774 (class 2606 OID 35421)
-- Name: work_types work_types_pkey; Type: CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.work_types
    ADD CONSTRAINT work_types_pkey PRIMARY KEY (id);


--
-- TOC entry 4788 (class 2620 OID 35480)
-- Name: repair_orders trigger_update_cost; Type: TRIGGER; Schema: public; Owner: postgre
--

CREATE TRIGGER trigger_update_cost BEFORE INSERT OR UPDATE OF work_type_id, performer_id, hours_spent ON public.repair_orders FOR EACH ROW EXECUTE FUNCTION public.update_total_cost();


--
-- TOC entry 4783 (class 2606 OID 35406)
-- Name: customers customers_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.customers
    ADD CONSTRAINT customers_user_id_fkey FOREIGN KEY (user_id) REFERENCES public.users(id) ON DELETE CASCADE;


--
-- TOC entry 4784 (class 2606 OID 35438)
-- Name: performers performers_user_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.performers
    ADD CONSTRAINT performers_user_id_fkey FOREIGN KEY (user_id) REFERENCES public.users(id) ON DELETE CASCADE;


--
-- TOC entry 4785 (class 2606 OID 35465)
-- Name: repair_orders repair_orders_customer_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.repair_orders
    ADD CONSTRAINT repair_orders_customer_id_fkey FOREIGN KEY (customer_id) REFERENCES public.customers(id) ON DELETE CASCADE;


--
-- TOC entry 4786 (class 2606 OID 35475)
-- Name: repair_orders repair_orders_performer_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.repair_orders
    ADD CONSTRAINT repair_orders_performer_id_fkey FOREIGN KEY (performer_id) REFERENCES public.performers(id);


--
-- TOC entry 4787 (class 2606 OID 35470)
-- Name: repair_orders repair_orders_work_type_id_fkey; Type: FK CONSTRAINT; Schema: public; Owner: postgre
--

ALTER TABLE ONLY public.repair_orders
    ADD CONSTRAINT repair_orders_work_type_id_fkey FOREIGN KEY (work_type_id) REFERENCES public.work_types(id);


-- Completed on 2026-04-21 19:25:18

--
-- PostgreSQL database dump complete
--

\unrestrict lywVrlHm07U16Ct4kTYSi5DNALEcg7lRI4IA0GoPHs3e8iphvbCl5J0zlRu4Mof

