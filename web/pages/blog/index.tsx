
import type { NextPage } from 'next'
import Head from 'next/head'
import Image from 'next/image'
import {Header} from '../../components/header/header'
import {PostOverview} from '../../components/posts/post-overview'
import styles from './Blog.module.css'

const Blog: NextPage = () => {
  return (
    <div className={styles.container}>
      <Head>
        <title>Bluebox blog</title>
        <meta name="description" content="The Bluebox development and annoucement blog!" />
      </Head>

      <main className={styles.main}>
        <Header />
        <h1 className={styles.title}>Latest</h1>
        <p className={styles.description}>A blog about the development of a bluetooth guitar pedal. Also covers product & feature announcements.</p>
        
       <PostOverview title={'Workstation Setup for Bluebox'} overview={'A quick overview of the setup I made for building the effects pedal.'} slug={'blog/posts/workstation'} tag0={'Oscilloscope'} tag1={'Soldering Iron'} tag2={'Power'} date={'Friday, August 12, 2022'} />
      </main>

      <footer className={styles.footer}>
      </footer>
    </div>
  )
}

export default Blog
